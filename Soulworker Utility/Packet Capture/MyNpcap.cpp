#include "pch.h"
#include ".\Packet Capture\MyNpcap.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Packet Capture\PacketCapture.h"
#include ".\UI\Option.h"
#include ".\Third Party\Npcap\Include\pcap.h"
#include ".\Soulworker Packet\PacketType.h"
#include "SWConfig.h"

DWORD MyNpcap::LoadNpcapDlls() {

	DWORD error = ERROR_SUCCESS;

	do {
		TCHAR npcap_dir[MAX_PATH + 3];
		UINT len;

		len = GetSystemDirectory(npcap_dir, MAX_PATH);

		if (!len) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in GetSystemDirectory: %x")), GetLastError());
			error = ERROR_FUNCTION_FAILED;
			break;
		}

		_tcscat_s(npcap_dir, MAX_PATH, TEXT("\\Npcap"));

		if (SetDllDirectory(npcap_dir) == 0) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in SetDllDirectory: %x")), GetLastError());
			error = ERROR_FUNCTION_FAILED;
			break;
		}

		HINSTANCE hDLL = LoadLibrary(L"wpcap.dll");
		if (hDLL == NULL) {
			//Log::WriteLog(const_cast<LPTSTR>(_T("LoadLibrary wpcap.dll failed. %x")), GetLastError());
			error = ERROR_API_UNAVAILABLE;
			break;
		}

		void* pWpcap = GetProcAddress(hDLL, "pcap_compile");
		if (pWpcap == NULL) {
			Log::WriteLog(const_cast<LPTSTR>(_T("GetProcAddress wpcap.dll failed. %x")), GetLastError());
			error = ERROR_API_UNAVAILABLE;
			break;
		}

		FreeLibrary(hDLL);

	} while (false);

	return error;
}

DWORD MyNpcap::Init() {

	DWORD error = ERROR_SUCCESS;

	do {
		if (LoadNpcapDlls()) {
			error = ERROR_DLL_NOT_FOUND;
			break;
		}

		if (_inited)
			StopSniffAllInterface();

		sniffAllInterface(NPCAP_FILTER_RULE);

	} while (false);

	return error;
}

VOID MyNpcap::sniffAllInterface(string bpfFilter)
{
	// sniff all interface
	for (auto itr = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().begin(); itr != pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().end(); itr++)
	{
		const CHAR* ifName = UIOPTION.GetUseInterface();
		// skip \DEVICE\NPF_
		if (strcmp(ifName, "ALL") != 0 && strcmp(ifName, (*itr)->getName().c_str() + 12) != 0)
			continue;
		if (strcmp("Loopback", (*itr)->getName().c_str() + 12) == 0)
			continue;

		ThreadInfo* ti = new ThreadInfo;
		ti->_this = this;
		ti->dev = *itr;
		ti->bpfFilter = bpfFilter;

		// start capturing packets and do TCP reassembly
		HANDLE h = CreateThread(NULL, 0, doTcpReassemblyOnLiveTraffic, ti, 0, NULL);
		if (h != NULL)
			CloseHandle(h);
	}

	_inited = TRUE;
}

VOID MyNpcap::StopSniffAllInterface()
{
	if (UIOPTION.GetCaptureMode() != (INT32)CaptureType::_NPCAP)
		return;

	_stopIfCount = 0;
	size_t createThread = 0;

	for (auto itr = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().begin(); itr != pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().end(); itr++)
	{
		if ((*itr)->captureActive())
		{
			// create stop sniff thread
			HANDLE h = CreateThread(NULL, 0, StopSniffThread, *itr, 0, NULL);
			if (h != NULL)
			{
				CloseHandle(h);
				createThread++;
			}
		}
	}

	while (TRUE)
	{
		if (_stopIfCount == createThread)
			break;
		else
			Sleep(100);
	}
}

DWORD MyNpcap::StopSniffThread(LPVOID Param)
{
	pcpp::PcapLiveDevice* pDevice = (pcpp::PcapLiveDevice*)Param;
	if (pDevice->captureActive())
	{
		pDevice->stopCapture();
	}

	NPCAP.AddStopIfCount();
	return 0;
}

/**
 * The method responsible for TCP reassembly on live traffic
 */
DWORD MyNpcap::doTcpReassemblyOnLiveTraffic(LPVOID param)
{
	ThreadInfo* ti = (ThreadInfo*)param;
	if (ti == NULL)
		return -99;
	MyNpcap* _this = ti->_this;

	// create the TCP reassembly instance
	TcpReassemblyCookie trcRecv{};
	trcRecv._isRecv = TRUE;
	trcRecv._flowKey = 0;
	pcpp::TcpReassembly tcpRecvReassembly(tcpReassemblyMsgReadyCallback, &trcRecv, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);
	TcpReassemblyCookie trcSend{};
	trcSend._isRecv = FALSE;
	trcSend._flowKey = 0;
	pcpp::TcpReassembly tcpSendReassembly(tcpReassemblyMsgReadyCallback, &trcSend, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);

	CaptureInfo ci{};
	ci.recvReassembly = &tcpRecvReassembly;
	ci.sendReassembly = &tcpSendReassembly;

	pcpp::PcapLiveDevice::DeviceConfiguration config;
	config.packetBufferTimeoutMs = 100;

	// try to open device
	if (!ti->dev->open(config))
	{
		Log::WriteLogA("[MyNpcap::doTcpReassemblyOnLiveTraffic] Open interface failed. Please check your Npcap version is 1.60 or above.");
		return -1;
	}

	// set BPF filter if set by the user
	if (!ti->bpfFilter.empty())
	{
		if (!ti->dev->setFilter(ti->bpfFilter))
			return -2;
	}

	// start capturing packets. Each packet arrived will be handled by onPacketArrives method
	if (!ti->dev->startCapture(onPacketArrives, &ci))
	{
		Log::WriteLogA("[MyNpcap::doTcpReassemblyOnLiveTraffic] startCapture failed. Please check your Npcap version is 1.60 or above.");
		return -3;
	}

	// run in an endless loop
	while (ti->dev->captureActive())
		Sleep(1000);

	// close all connections which are still opened
	tcpRecvReassembly.closeAllConnections();
	tcpSendReassembly.closeAllConnections();

	// clear remaining data
	if (trcRecv._remainingSize > 0)
		delete[] trcRecv._remainingData;
	if (trcSend._remainingSize > 0)
		delete[] trcSend._remainingData;

	return 0;
}

/**
 * packet capture callback - called whenever a packet arrives on the live device (in live device capturing mode)
 */
void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* tcpReassemblyCookie)
{
	CaptureInfo* pCI = (CaptureInfo*)tcpReassemblyCookie;
	pcpp::Packet parsedPacket(packet);
	pcpp::TcpLayer* tcpLayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
	pcpp::TcpReassembly* tcpReassembly = pCI->sendReassembly;
	const BOOL isRecv = tcpLayer->getSrcPort() == 10200;
	
	if (tcpLayer == NULL || _stopNpcap)
		return;

	if (isRecv)
	{
		tcpReassembly = pCI->recvReassembly;
	}

	// get a pointer to the TCP reassembly instance and feed the packet arrived to it
	pcpp::TcpReassembly::ReassemblyStatus status = tcpReassembly->reassemblePacket(packet);

	if (isRecv)
		PACKETCAPTURE.UpdateLoss(status == pcpp::TcpReassembly::ReassemblyStatus::OutOfOrderTcpMessageBuffered);
}

/**
 * The callback being called by the TCP reassembly module whenever new data arrives on a certain connection
 */
VOID tcpReassemblyMsgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData& tcpData, void* userCookie)
{
	TcpReassemblyCookie* pTRC = (TcpReassemblyCookie*)userCookie;
	const BOOL isRecv = pTRC->_isRecv;
	BOOL reassembly = FALSE;
	BOOL isCurrentFlow = (pTRC->_flowKey == 0 || pTRC->_flowKey == tcpData.getConnectionData().flowKey);

	// parse timestamp
	CHAR tmp[128] = { 0 };
	auto old_uses = std::to_string(tcpData.getTimeStamp().tv_usec);
	auto new_usec = std::string(6 - min(6, old_uses.length()), '0') + old_uses;
	sprintf_s(tmp, "%d%s", tcpData.getTimeStamp().tv_sec, new_usec.c_str());

	IPv4Packet packet{};
	packet._data = (uint8_t*)tcpData.getData();
	packet._datalength = tcpData.getDataLength();
	if (pTRC->_remainingSize > 0 && isCurrentFlow)
	{
		reassembly = TRUE;
		packet._datalength = pTRC->_remainingSize + tcpData.getDataLength();
		packet._data = new uint8_t[packet._datalength];

		// copy remaining data to buffer
		memcpy_s((void*)packet._data, packet._datalength, pTRC->_remainingData, pTRC->_remainingSize);
		// copy current packet to buffer
		memcpy_s((void*)(packet._data + pTRC->_remainingSize), packet._datalength, tcpData.getData(), tcpData.getDataLength());

		delete[] pTRC->_remainingData;
		pTRC->_remainingSize = 0;

#if DEBUG_NPCAP_REASSEMBLY == 1
		Log::WriteLogA("[tcpReassemblyMsgReadyCallback] rewrite packet, oldlen = %llu, newlen = %llu", tcpData.getDataLength(), packet._datalength);
		for (int i = 0; i < packet._datalength - tcpData.getDataLength(); i++)
			Log::WriteLogNoDate(L"%02x ", packet._data[i]);
		Log::WriteLogNoDate(L"\n\n");
#endif
	}
	packet._isRecv = isRecv;
	packet._ts = atoll(tmp) / 1000;
	packet._pkt = packet._data;

	PACKETPARSER.Parse(&packet, isRecv);

	int type = 0;
	size_t maxRemainingLen = sizeof(SWSHEADER);
	if (isRecv)
		maxRemainingLen = sizeof(SWHEADER);
	// check remaining data is SW packet
	do
	{
		if (packet._datalength > 0 && packet._datalength < maxRemainingLen && isCurrentFlow && *packet._data == SWMAGIC)
		{
			// check SWMAGIC second byte
			if (packet._datalength > 1 && *(packet._data + 1) != 0)
			{
				type = 2;
				break;
			}
			// check Length
			if (packet._datalength > 2 && *(packet._data + 2) == 0)
			{
				type = 3;
				break;
			}
			// check const
			if (packet._datalength > 4)
			{
				const BYTE constVal = *(packet._data + 4);
				if (constVal != SWCONSTVALUE_RECV && constVal != SWCONSTVALUE_SEND)
				{
					type = 4;
					break;
				}
			}

			pTRC->_remainingData = new uint8_t[packet._datalength];
			pTRC->_remainingSize = packet._datalength;
			// copy remaining data to cookie
			memcpy_s(pTRC->_remainingData, pTRC->_remainingSize, packet._data, pTRC->_remainingSize);
#if DEBUG_NPCAP_REASSEMBLY == 1
			Log::WriteLogA("[tcpReassemblyMsgReadyCallback] find remaining data, len = %llu", pTRC->_remainingSize);
#endif
		}
		else if (packet._datalength > 0) {
			type = 1;
			break;
		}
	} while (false);

	if (type > 0)
	{
#if DEBUG_NPCAP_REASSEMBLY == 1
		Log::WriteLogA("[tcpReassemblyMsgReadyCallback] Skip. isCurrentFlow = %d, type = %d, maybe isnt sw packet, len = %llu", (isCurrentFlow ? 1 : 0), type, packet._datalength);
		for (int i = 0; i < packet._datalength; i++)
			Log::WriteLogNoDate(L"%02x ", packet._data[i]);
		Log::WriteLogNoDate(L"\n\n");
#endif
	}
	else {
		pTRC->_flowKey = tcpData.getConnectionData().flowKey;
	}

	if (reassembly)
		delete[] packet._pkt;
}


/**
 * The callback being called by the TCP reassembly module whenever a new connection is found. This method adds the connection to the connection manager
 */
void tcpReassemblyConnectionStartCallback(const pcpp::ConnectionData& connectionData, void* userCookie)
{
}


/**
 * The callback being called by the TCP reassembly module whenever a connection is ending. This method removes the connection from the connection manager
 */
void tcpReassemblyConnectionEndCallback(const pcpp::ConnectionData& connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void* userCookie)
{
}