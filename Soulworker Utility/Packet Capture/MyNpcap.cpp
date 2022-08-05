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

		sniffAllInterface(NPCAP_FILTER_RULE);

	} while (false);

	return error;
}

VOID MyNpcap::sniffAllInterface(string bpfFilter)
{
	// sniff all interface
	for (auto itr = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().begin(); itr != pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().end(); itr++)
	{
		ThreadInfo* ti = new ThreadInfo;
		ti->_this = this;
		ti->dev = *itr;
		ti->bpfFilter = bpfFilter;

		// start capturing packets and do TCP reassembly
		HANDLE h = CreateThread(NULL, 0, doTcpReassemblyOnLiveTraffic, ti, 0, NULL);
		if (h != NULL)
			CloseHandle(h);
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
	pcpp::TcpReassembly tcpReassembly(tcpReassemblyMsgReadyCallback, NULL, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);

	// try to open device
	if (!ti->dev->open())
		return -1;

	// set BPF filter if set by the user
	if (!ti->bpfFilter.empty())
	{
		if (!ti->dev->setFilter(ti->bpfFilter))
			return -2;
	}

	// start capturing packets. Each packet arrived will be handled by onPacketArrives method
	ti->dev->startCapture(_this->onPacketArrives, &tcpReassembly);

	// register the on app close event to print summary stats on app termination
	bool shouldStop = false;
	pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted(_this->onApplicationInterrupted, &shouldStop);

	// run in an endless loop until the user presses ctrl+c
	while (!shouldStop)
		pcpp::multiPlatformSleep(1);

	// stop capturing and close the live device
	ti->dev->stopCapture();
	ti->dev->close();

	// close all connections which are still opened
	tcpReassembly.closeAllConnections();

	return 0;
}

/**
 * The callback to be called when application is terminated by ctrl-c. Stops the endless while loop
 */
void MyNpcap::onApplicationInterrupted(void* cookie)
{
	bool* shouldStop = (bool*)cookie;
	*shouldStop = true;
}


/**
 * packet capture callback - called whenever a packet arrives on the live device (in live device capturing mode)
 */
void MyNpcap::onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* tcpReassemblyCookie)
{
	pcpp::Packet parsedPacket(packet);
	pcpp::TcpLayer* tcpLayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();

	if (tcpLayer == NULL)
		return;

	if (tcpLayer->getSrcPort() == 10200)
	{
		// get a pointer to the TCP reassembly instance and feed the packet arrived to it
		pcpp::TcpReassembly* tcpReassembly = (pcpp::TcpReassembly*)tcpReassemblyCookie;
		pcpp::TcpReassembly::ReassemblyStatus status = tcpReassembly->reassemblePacket(packet);
		PACKETCAPTURE.UpdateLoss(status == pcpp::TcpReassembly::ReassemblyStatus::OutOfOrderTcpMessageBuffered);
	} 
	else if (tcpLayer->getLayerPayloadSize() > 0)
	{
		IPv4Packet packet;
		packet._data = tcpLayer->getLayerPayload();
		packet._datalength = tcpLayer->getLayerPayloadSize();
		packet._isRecv = FALSE;
		packet._pkt = (BYTE*)packet._data;
		packet._ts = GetCurrentTimeStamp();

		PACKETPARSER.Parse(&packet, packet._isRecv);
	}
}

/**
 * The callback being called by the TCP reassembly module whenever new data arrives on a certain connection
 */
VOID MyNpcap::tcpReassemblyMsgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData& tcpData, void* userCookie)
{
	CHAR tmp[128] = { 0 };
	auto old_uses = std::to_string(tcpData.getTimeStamp().tv_usec);
	// padding zero
	auto new_usec = std::string(6 - min(6, old_uses.length()), '0') + old_uses;
	sprintf_s(tmp, "%d%s", tcpData.getTimeStamp().tv_sec, new_usec.c_str());

	IPv4Packet packet;
	packet._data = tcpData.getData();
	packet._datalength = tcpData.getDataLength();
	packet._isRecv = (tcpData.getConnectionData().srcPort == 10200);
	packet._pkt = (BYTE*)packet._data;
	packet._ts = atoll(tmp) / 1000;
	
	PACKETPARSER.Parse(&packet, packet._isRecv);
}


/**
 * The callback being called by the TCP reassembly module whenever a new connection is found. This method adds the connection to the connection manager
 */
void MyNpcap::tcpReassemblyConnectionStartCallback(const pcpp::ConnectionData& connectionData, void* userCookie)
{
}


/**
 * The callback being called by the TCP reassembly module whenever a connection is ending. This method removes the connection from the connection manager
 */
void MyNpcap::tcpReassemblyConnectionEndCallback(const pcpp::ConnectionData& connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void* userCookie)
{
}