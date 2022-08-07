#include "pch.h"
#include ".\Packet Capture\MyNpcap.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Packet Capture\PacketCapture.h"
#include ".\UI\Option.h"
#include ".\Third Party\Npcap\Include\pcap.h"

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
	for (auto itr = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().begin(); itr != pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().end(); itr++)
	{
		if ((*itr)->captureActive())
		{
			(*itr)->stopCapture();
		}
	}
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
	pcpp::TcpReassembly tcpRecvReassembly(tcpReassemblyMsgReadyCallback, (void*)TRUE, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);
	pcpp::TcpReassembly tcpSendReassembly(tcpReassemblyMsgReadyCallback, FALSE, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);
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
	const BOOL isRecv = userCookie == (void*)1;
	CHAR tmp[128] = { 0 };
	auto old_uses = std::to_string(tcpData.getTimeStamp().tv_usec);
	// padding zero
	auto new_usec = std::string(6 - min(6, old_uses.length()), '0') + old_uses;
	sprintf_s(tmp, "%d%s", tcpData.getTimeStamp().tv_sec, new_usec.c_str());

	IPv4Packet packet{};
	packet._data = (const UCHAR*)tcpData.getData();
	packet._datalength = tcpData.getDataLength();
	packet._isRecv = isRecv;
	packet._ts = atoll(tmp) / 1000;
	packet._pkt = (BYTE*)tcpData.getData();

#if _DEBUG
	if (tcpData.isBytesMissing())
	{
		Log::WriteLogA("Find missing data");
		for (int i = 0; i < packet._datalength; i++)
			Log::WriteLogNoDate(L"%02x ", packet._pkt[i]);
		Log::WriteLogNoDate(L"\n\n");
	}
	char* encryptData = new char[tcpData.getDataLength()];
	memcpy_s(encryptData, tcpData.getDataLength(), tcpData.getData(), tcpData.getDataLength());
	packet._encryptData = (const UCHAR*)encryptData;
	packet._encryptDataLen = tcpData.getDataLength();
#endif

	PACKETPARSER.Parse(&packet, isRecv);

	if (packet._encryptData != nullptr)
		delete[] packet._encryptData;
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