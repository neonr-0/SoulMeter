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

DWORD MyNpcap::Filter(pcap_t* device) {

	DWORD error = ERROR_SUCCESS;

	do {

		if (device == nullptr) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		bpf_program fcode;

		if (pcap_compile(device, &fcode, NPCAP_FILTER_RULE, 1, NULL) < 0) {
			error = ERROR_API_UNAVAILABLE;
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in pcap_compile")));
			break;
		}

		if (pcap_setfilter(device, &fcode) < 0) {
			error = ERROR_API_UNAVAILABLE;
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in pcap_setfilter")));
			break;
		}

	} while (false);

	return error;
}

DWORD MyNpcap::Init() {

	DWORD error = ERROR_SUCCESS;

	pcap_if_t* alldevs = nullptr;

	char temp[MAX_BUFFER_LENGTH] = { 0 };

	do {
		if (LoadNpcapDlls()) {
			error = ERROR_DLL_NOT_FOUND;
			break;
		}

		if (pcap_findalldevs_ex(const_cast<char*>(PCAP_SRC_IF_STRING), NULL, &alldevs, temp) == -1) {
			Log::WriteLogA(temp);
			error = ERROR_API_UNAVAILABLE;
			break;
		}

		for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {

			pcap_t* device = nullptr;

			if ((device = pcap_open(d->name, 65536, PCAP_OPENFLAG_PROMISCUOUS | PCAP_OPENFLAG_NOCAPTURE_RPCAP, 10, NULL, temp)) == NULL)
				continue;

			if (Filter(device))
				continue;

			ThreadInfo* ti = new ThreadInfo;
			ti->_device = device;
			ti->_thisl = this;

			CreateThread(NULL, 0, CreatePcapLoop, ti, 0, NULL);
		}

		pcap_freealldevs(alldevs);

	} while (false);

	return error;
}

DWORD MyNpcap::CreatePcapLoop(LPVOID pAlldevs)
{
	ThreadInfo* ti = (ThreadInfo*)pAlldevs;

	pcap_loop(ti->_device, 0, ReceiveCallback, nullptr);
	/*int res;
	pcap_pkthdr* header = nullptr;
	const unsigned char* pkt_data = nullptr;
	while (TRUE)
	{
		res = pcap_next_ex(ti->_device, &header, &pkt_data);
		if (res == 1)
			ReceiveCallback((u_char*)ti->_thisl, header, pkt_data);
		else
			Sleep(10);
	}*/

	return 0;
}

VOID MyNpcap::ReceiveCallback(u_char* prc, const struct pcap_pkthdr* header, const u_char* pkt_data) {

	if (prc != nullptr || pkt_data == nullptr || header == nullptr || PACKETCAPTURE.isStopCapture()) {
		return;
	}

	BYTE* new_pkt_data = new BYTE[header->caplen];
	memcpy(new_pkt_data, pkt_data, header->caplen);

	CHAR tmp[128] = { 0 };
	sprintf_s(tmp, "%d%d", header->ts.tv_sec, header->ts.tv_usec);

	IPv4Packet* packet = new IPv4Packet;
	PACKETCAPTURE.ParseNpcapStruct(packet, (BYTE*)new_pkt_data, (pcap_pkthdr*)header);

#if DEBUG_NPCAP_SORT == 1
	Log::WriteLogA("[MyNpcap::ReceiveCallback] SEQ = %lu, CapLen = %lu, Length = %lu", packet->_tcpHeader->seq_number, header->caplen, header->len);
	//if (header->caplen > 1300) {
	//	for (SIZE_T i = 0; i < header->caplen; i++)
	//		Log::WriteLogNoDate(L"%02X", pkt_data[i]);
	//	Log::WriteLogNoDate(L"\n\n");
	//}
#endif

	recursive_mutex* pMutex = nullptr;
	if (packet->_isRecv) {
		pMutex = PACKETCAPTURE.GetRecvMutex();
	}
	else {
		pMutex = PACKETCAPTURE.GetSendMutex();
	}

	pMutex->lock();
	if (packet->_tcpHeader->syn) {
#if DEBUG_NPCAP_SORT == 1
		Log::WriteLogA("[MyNpcap::ReceiveCallback] SEQ = %lu, ClearQueue", packet->_tcpHeader->seq_number);
#endif
		PACKETCAPTURE.SetSEQ(packet->_tcpHeader->seq_number + 1, packet->_isRecv);
	}
	else if (!PACKETCAPTURE.isInitRecv() || !PACKETCAPTURE.isInitSend()) {
#if DEBUG_NPCAP_SORT == 1
		Log::WriteLogA("[MyNpcap::ReceiveCallback] SEQ = %lu, %s", packet->_tcpHeader->seq_number, packet->_isRecv ? "isInitRecv" : "isInitSend");
#endif
		PACKETCAPTURE.SetSEQ(packet->_tcpHeader->seq_number, packet->_isRecv);
	}

	if (packet->_datalength > 0 && packet->_tcpHeader->ack)
	{
		PacketInfo* pi = new PacketInfo;
		pi->_packet = packet;
		pi->_ts = GetCurrentTimeStamp();

		PACKETCAPTURE.InsertQueue(packet->_tcpHeader->seq_number, pi, packet->_isRecv);
	}
	else {
		delete packet;
		delete[] new_pkt_data;
	}

	pMutex->unlock();

}