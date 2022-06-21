#include "pch.h"
#include ".\Packet Capture\MyWinDivert.h"
#include ".\Soulworker Packet\SWPacketMaker.h"
#include ".\Soulworker Packet\SWSPacketMaker.h"
#include ".\Soulworker Packet\SWCrypt.h"
#include ".\UI\Option.h"

//	 Windivert 1.4.2
#define WINDIVERT_MTU_MAX (40 + 0xFFFF)

DWORD MyWinDivert::Init() {

	DWORD error = ERROR_SUCCESS;

	do {
		_handle = WinDivertOpen(WINDIVERT_FILTER_RULE, WINDIVERT_LAYER_NETWORK, 0, WINDIVERT_FLAG_SNIFF);

		if (_handle == INVALID_HANDLE_VALUE) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertOpen: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		if (!WinDivertSetParam(_handle, WINDIVERT_PARAM_QUEUE_LEN, 16384))
		{
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertSetParam1: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		if (!WinDivertSetParam(_handle, WINDIVERT_PARAM_QUEUE_TIME, 8000))
		{
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertSetParam2: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		if (!WinDivertSetParam(_handle, WINDIVERT_PARAM_QUEUE_SIZE, 33554432))
		{
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertSetParam3: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		CreateThread(NULL, 0, ReceiveCallback, this, 0, NULL);

	} while (false);

	return error;
}

DWORD MyWinDivert::ReceiveCallback(LPVOID prc) {

	DWORD error = ERROR_SUCCESS;

	do {

		if (prc == nullptr) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		MyWinDivert* _this = (MyWinDivert*)prc;

		WINDIVERT_ADDRESS addr;
		UINT addrlen = sizeof(WINDIVERT_ADDRESS);
		UINT recvlen = 0;

		while (TRUE) {

			BYTE* pkt_data = new BYTE[WINDIVERT_MTU_MAX];
			// Windivert 1.4.2
			if (!WinDivertRecvEx(_this->_handle, pkt_data, WINDIVERT_MTU_MAX, 0, &addr, &recvlen, NULL)) {
				// WinDivert 2.2
				//if (!WinDivertRecvEx(_this->_handle, pkt_data, packetlen, &recvlen, 0, &addr, &addrlen, NULL)) {

				Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertRecv : %x")), GetLastError());
				continue;
			}

			PacketInfo* pi = new PacketInfo;
			pi->_pkt = pkt_data;
			pi->_this = _this;

			HANDLE ct = CreateThread(0, NULL, ParsePacket, pi, 0, NULL);
			if (ct == NULL) {
				Log::WriteLog(const_cast<LPTSTR>(_T("Error in CreateParsePacketThread : %x")), GetLastError());
				break;
			}
		}

	} while (false);

	return error;
}

DWORD MyWinDivert::ParsePacket(LPVOID prc) 
{
	PacketInfo* pi = (PacketInfo*)prc;
	if (pi->_this == nullptr)
		return -1;

	std::lock_guard<std::mutex> lock(pi->_this->_mutex);

	IPv4Packet packet = { 0 };
	// IP
	packet._ipHeader = (IPHEADER*)(pi->_pkt);
	packet._ipHeader->length = _byteswap_ushort(packet._ipHeader->length);

	// TCP
	packet._tcpHeader = (TCPHEADER*)(pi->_pkt + packet._ipHeader->len * 4);

	packet._datalength = packet._ipHeader->length - (packet._ipHeader->len * 4 + packet._tcpHeader->length * 4);
	packet._data = (pi->_pkt + packet._ipHeader->len * 4 + packet._tcpHeader->length * 4);

	USHORT realSrcPort = _byteswap_ushort(packet._tcpHeader->src_port);
	USHORT realDstPort = _byteswap_ushort(packet._tcpHeader->dest_port);
	BOOL isRecv = (realSrcPort == 10200);

#if DEBUG_DIVERT_ALL == 1
	PrintIPHeader(&packet);
	PrintTCPHeader(&packet);

	printf("[Packet Data Start]\n");
	for (int i = 0; i < packet._datalength; i++)
		printf("%02x ", packet._data[i]);
	printf("\n");
	printf("[Packet Data End]\n");
#endif
#if DEBUG_DIVERT_IP == 1
	PrintIPHeader(&packet);
#endif

#if DEBUG_DIVERT_TCP == 1
	PrintTCPHeader(&packet);
#endif

#if DEBUG_DIVERT_DATA == 1
	Log::WriteLogA("[%s Packet Data Start]", isRecv ? "RECV" : "SEND");
	for (int i = 0; i < packet._datalength; i++)
		Log::WriteLogNoDate(L"%02x ", packet._data[i]);
	Log::WriteLogNoDate(L"\n");
	Log::WriteLogA("[Packet Data End]");
#endif

	if (isRecv)
		SWPACKETMAKER.Parse(&packet);
	else
		SWSPACKETMAKER.Parse(&packet);

	delete[] pi->_pkt;
	delete pi;

	return 0;
}

VOID MyWinDivert::PrintIPHeader(IPv4Packet* p_packet) {

	if (p_packet == nullptr)
		return;

	IPHEADER* ih = p_packet->_ipHeader;

	printf("======== IP Header ========\n");
	printf("Version : %d\n", ih->version);
	printf("len : %d\n", ih->len * 4);
	printf("length : %d\n", ih->length);
	printf("TTL : %d\n", ih->ttl);
	printf("protocol : %d\n", ih->protocol);
	printf("Src IP  : %d.%d.%d.%d\n", ih->src_ip & 0xFF, (ih->src_ip >> 8) & 0xFF, (ih->src_ip >> 16) & 0xF, (ih->src_ip >> 24) & 0xFF);
	printf("Dst IP  : %d.%d.%d.%d\n", ih->dest_ip & 0xFF, (ih->dest_ip >> 8) & 0xFF, (ih->dest_ip >> 16) & 0xF, (ih->dest_ip >> 24) & 0xFF);

	return;
}

VOID MyWinDivert::PrintTCPHeader(IPv4Packet* p_packet) {

	if (p_packet == nullptr)
		return;

	TCPHEADER* th = p_packet->_tcpHeader;

	printf("======== TCP Header ========\n");
	printf("src_port : %u\n", _byteswap_ushort(th->src_port));
	printf("dest_port : %u\n", _byteswap_ushort(th->dest_port));
	printf("length : %d\n", th->length * 4);
}