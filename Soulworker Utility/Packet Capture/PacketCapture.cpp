#include "pch.h"
#include ".\Packet Capture\PacketCapture.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Packet Capture\MyWinDivert.h"
#include ".\Packet Capture\MyNpcap.h"
#include ".\UI\Option.h"

BOOL PacketCapture::Init() {

	BOOL error = FALSE;

	_useType = CaptureType::_NPCAP;
	if (NPCAP.Init())
		error = TRUE;

	if (error) {
		_useType = CaptureType::_WINDIVERT;
		error = WINDIVERT.Init(_handle) > 0;
	}

	if (!error) {
		for (BYTE i = 0; i < 2; i++)
		{
			ParseThreadInfo* pti = new ParseThreadInfo;
			pti->isRecv = (i == 0);
			pti->_this = this;

			HANDLE ct = CreateThread(NULL, 0, PacketRoute, pti, 0, NULL);
			if (ct == nullptr)
				return FALSE;
		}
	}

	return error;
}

DWORD WINAPI PacketCapture::PacketRoute(LPVOID prc)
{
	ParseThreadInfo* pti = (ParseThreadInfo*)prc;
	PacketCapture* _this = pti->_this;

	if (_this == nullptr) {
		return -1;
	}

	unordered_map<ULONG, PacketInfo*>* queue = nullptr;
	ULONG* nextSEQ = nullptr;
	mutex* mutex = nullptr;

	if (pti->isRecv) {
		queue = &_this->_recvPacketQueue;
		nextSEQ = &_this->_nextRecvSEQ;
		mutex = &_this->_recvMutex;
	}
	else {
		queue = &_this->_sendPacketQueue;
		nextSEQ = &_this->_nextSendSEQ;
		mutex = &_this->_sendMutex;
	}

	BOOL skipCheckSEQ = FALSE;
	if (_this->_useType == CaptureType::_WINDIVERT)
		skipCheckSEQ = TRUE;

	while (TRUE)
	{
		PacketInfo* pi = nullptr;

		if (_this->_pauseParse) 
		{
			Sleep(100);
			continue;
		}

		mutex->lock();
		for (auto itr = queue->begin(); itr != queue->end(); itr++) 
		{
			if (skipCheckSEQ || itr->first == *nextSEQ) {
#if DEBUG_CAPTURE_SORT == 1
				Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] [skipCheckSEQ:%s] Find Packet SEQ %lu, PacketLen = %lu", pti->isRecv ? "RECV" : "SEND", skipCheckSEQ ? "YES" : "NO", itr->first, itr->second->_packet->_datalength);
#endif
				pi = itr->second;
				queue->erase(itr);
				break;
			}
		}

		if (pi != nullptr) {
			*nextSEQ += static_cast<ULONG>(pi->_packet->_datalength);

#if DEBUG_CAPTURE_SORT == 1
			Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] Set next SEQ %lu, Queue length: %d", pti->isRecv ? "RECV" : "SEND", *nextSEQ, queue->size());
#endif

			ParsePacket(_this, pi);

			ClearPacketInfo(pi);
		}
		else {
			Sleep(10);
		}
		mutex->unlock();
	}

	return 0;
}

DWORD PacketCapture::ParsePacket(LPVOID prc, PacketInfo* pi)
{
	PacketCapture* _this = (PacketCapture*)prc;

	if (_this == nullptr || pi->_packet == nullptr) {
		return -1;
	}

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

	PACKETPARSER.Parse(pi->_packet, pi->_packet->_isRecv);

	return 0;
}

VOID PacketCapture::ParseWinDivertStruct(IPv4Packet* packet, BYTE* pkt)
{
	// IP
	packet->_ipHeader = (IPHEADER*)(pkt);
	packet->_ipHeader->length = _byteswap_ushort(packet->_ipHeader->length);

	// TCP
	packet->_tcpHeader = (TCPHEADER*)(pkt + packet->_ipHeader->len * 4);
	packet->_tcpHeader->seq_number = _byteswap_ulong(packet->_tcpHeader->seq_number);
	packet->_tcpHeader->src_port = _byteswap_ushort(packet->_tcpHeader->src_port);
	packet->_tcpHeader->dest_port = _byteswap_ushort(packet->_tcpHeader->dest_port);

	packet->_datalength = packet->_ipHeader->length - (packet->_ipHeader->len * 4 + packet->_tcpHeader->length * 4);
	packet->_data = (pkt + packet->_ipHeader->len * 4 + packet->_tcpHeader->length * 4);

	packet->_isRecv = (packet->_tcpHeader->src_port == 10200);

	packet->_ts = GetCurrentTimeStamp();

	packet->_pkt = pkt;
}

VOID PacketCapture::ParseNpcapStruct(IPv4Packet* packet, BYTE* pkt, UINT32 capLen)
{
	packet->_ethernetHeader = (ETHERNETHEADER*)pkt;

	packet->_ipHeader = (IPHEADER*)(pkt + 14);
	packet->_ipHeader->length = _byteswap_ushort(packet->_ipHeader->length);

	packet->_tcpHeader = (TCPHEADER*)(pkt + 14 + packet->_ipHeader->len * 4);
	packet->_tcpHeader->seq_number = _byteswap_ulong(packet->_tcpHeader->seq_number);
	packet->_tcpHeader->src_port = _byteswap_ushort(packet->_tcpHeader->src_port);
	packet->_tcpHeader->dest_port = _byteswap_ushort(packet->_tcpHeader->dest_port);
	packet->_isRecv = (packet->_tcpHeader->src_port == 10200);

	packet->_data = (pkt + 14 + packet->_ipHeader->len * 4 + packet->_tcpHeader->length * 4);

	// http://en.wikipedia.org/wiki/Ethernet_frame#Payload
	// 0 = padding
	BOOL isDataPacket = (packet->_isRecv && capLen > 60) || (!packet->_isRecv && capLen > 54);
	if (isDataPacket || *packet->_data != 0x0)
	{
		packet->_datalength = capLen - (14 + packet->_ipHeader->len * 4 + packet->_tcpHeader->length * 4);
		// Find padding length
		if (!isDataPacket) {
			UCHAR* pos = (UCHAR*)memchr(packet->_data, 0, packet->_datalength);
			if (pos != NULL)
				packet->_datalength = pos - packet->_data;
		}
	}
	else {
		packet->_datalength = 0;
	}

	packet->_ts = GetCurrentTimeStamp();

	packet->_pkt = pkt;
}

VOID PacketCapture::PrintIPHeader(IPv4Packet* p_packet) {

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

VOID PacketCapture::PrintTCPHeader(IPv4Packet* p_packet) {

	if (p_packet == nullptr)
		return;

	TCPHEADER* th = p_packet->_tcpHeader;

	if (th->src_port != 10200)
		return;

	Log::WriteLogA("======== TCP Header ========");
	Log::WriteLogA("src_port : %u", th->src_port);
	Log::WriteLogA("dest_port : %u", th->dest_port);
	Log::WriteLogA("seq : %u", th->seq_number);
	Log::WriteLogA("length : %d\n", th->length * 4);
	Log::WriteLogA("data_length : %d\n", p_packet->_datalength);
}

VOID PacketCapture::ClearPacketInfo(PacketInfo* pi)
{
	delete[] pi->_packet->_pkt;
	delete pi->_packet;
	delete pi;
}

VOID PacketCapture::ClearQueue(BOOL isRecv)
{
	unordered_map<ULONG, PacketInfo*>* queue = nullptr;

	if (isRecv) {
		queue = &_recvPacketQueue;
	}
	else {
		queue = &_sendPacketQueue;
	}

	for (auto itr = queue->begin(); itr != queue->end(); itr++) {
		ClearPacketInfo(itr->second);
	}

	queue->clear();
}