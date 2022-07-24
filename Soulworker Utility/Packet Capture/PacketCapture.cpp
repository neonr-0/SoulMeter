#include "pch.h"
#include ".\Packet Capture\PacketCapture.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Packet Capture\MyWinDivert.h"
#include ".\Packet Capture\MyNpcap.h"

BOOL PacketCapture::Init() {

	BOOL error = FALSE;
	DWORD errorCode = 0;

	_useMode = UIOPTION.GetCaptureMode();

	if (_useMode == (INT32)CaptureType::_NPCAP) {
		error = NPCAP.Init() > 0;
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
	}

	if (error || _useMode == (INT32)CaptureType::_WINDIVERT)
		error = WINDIVERT.Init(_handle) > 0;

	return error;
}

DWORD WINAPI PacketCapture::PacketRoute(LPVOID prc)
{
	ParseThreadInfo* pti = (ParseThreadInfo*)prc;
	PacketCapture* _this = pti->_this;

	if (_this == nullptr) {
		return -1;
	}

	map<ULONG, PacketInfo*>* queue = nullptr;
	ULONG* nextSEQ = nullptr;
	recursive_mutex* mutex = nullptr;

	INT32 retry = 0;
	INT32 packetOffset = 0;
	UCHAR tmpPacket[2048] = { 0 };
	SIZE_T tmpLen = 2048;
	while (TRUE)
	{
		PacketInfo* pi = nullptr;

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

		if (_this->_pauseParse || queue->empty())
		{
			Sleep(100);
			continue;
		}

		mutex->lock();
		for (auto itr = queue->begin(); itr != queue->end(); itr++)
		{
			if (itr->second != nullptr && itr->second->_packet != nullptr)
			{
				if (itr->first == *nextSEQ) {
#if DEBUG_CAPTURE_SORT == 1
					Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] Find Packet SEQ %lu, PacketLen = %lu", pti->isRecv ? "RECV" : "SEND", itr->first, itr->second->_packet->_datalength);
#endif
					pi = itr->second;
					queue->erase(itr);
					retry = 0;
					break;
				}
				/*else if (pti->isRecv && itr == queue->begin()) {
					if (retry > 50 && itr->first < *nextSEQ)
					{
						ULONG itrSEQ = itr->first;
						PacketInfo* itrPi = itr->second;
						ULONG offset = *nextSEQ - itrSEQ;
						// TCP Timeout retransmission
						if (itrPi->_packet->_datalength > offset && memcmp(itrPi->_packet->_data, tmpPacket + tmpLen - offset, offset) == 0) {
#if DEBUG_CAPTURE_SORT == 1
							Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] oldSEQ = %lu, findSEQ = %lu, nextSEQ = %lu, retry = %d", pti->isRecv ? "RECV" : "SEND", *nextSEQ - offset, itrSEQ, *nextSEQ + offset, retry);
#endif
							packetOffset = offset;
							*nextSEQ -= packetOffset;
							_this->_loss++;
						}
					}
				}*/
				if (itr == queue->end())
					break;
			}
		}
#if DEBUG_CAPTURE_QUEUE == 1
		Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] Queue length: %d", pti->isRecv ? "RECV" : "SEND", queue->size());
#endif
		mutex->unlock();

		if (pi != nullptr && pi->_packet != nullptr) {

			*nextSEQ += static_cast<ULONG>(pi->_packet->_datalength);

			if (pti->isRecv) {
				if (packetOffset > 0) {
					pi->_packet->_datalength -= packetOffset;
					pi->_packet->_data += packetOffset;
#if DEBUG_CAPTURE_SORT == 1
					Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] Find packetOffset = %d", pti->isRecv ? "RECV" : "SEND", packetOffset);
#endif
					packetOffset = 0;
				}

#if DEBUG_CAPTURE_SORT == 1
				Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] Set next SEQ %lu", pti->isRecv ? "RECV" : "SEND", *nextSEQ);
#endif
				tmpLen = pi->_packet->_datalength;
				ZeroMemory(tmpPacket, 2048);
				memcpy_s(tmpPacket, tmpLen, pi->_packet->_data, tmpLen);
			}

			ParsePacket(_this, pi);
			ClearPacketInfo(pi);
		}
		else {
			retry++;
			Sleep(100);
			continue;
		}
		retry = 0;
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

VOID PacketCapture::ParseNpcapStruct(IPv4Packet* packet, BYTE* pkt, pcap_pkthdr* pkthdr)
{
	SHORT offset = 14;

	packet->_ethernetHeader = (ETHERNETHEADER*)pkt;
	//if (packet->_ethernetHeader->Type != 8) // check is raw packet
	//	offset = 0;

	packet->_ipHeader = (IPHEADER*)(pkt + offset);
	packet->_ipHeader->length = _byteswap_ushort(packet->_ipHeader->length);

	packet->_tcpHeader = (TCPHEADER*)(pkt + offset + packet->_ipHeader->len * 4);
	packet->_tcpHeader->seq_number = _byteswap_ulong(packet->_tcpHeader->seq_number);
	packet->_tcpHeader->ack_number = _byteswap_ulong(packet->_tcpHeader->ack_number);
	packet->_tcpHeader->src_port = _byteswap_ushort(packet->_tcpHeader->src_port);
	packet->_tcpHeader->dest_port = _byteswap_ushort(packet->_tcpHeader->dest_port);
	packet->_isRecv = (packet->_tcpHeader->src_port == 10200);

	USHORT packetHeaderLen = offset + packet->_ipHeader->len * 4 + packet->_tcpHeader->length * 4;

	packet->_data = (pkt + packetHeaderLen);

	// http://en.wikipedia.org/wiki/Ethernet_frame#Payload
	// 14 eth + 20 ip + 20 tcp = 54
	packet->_datalength = packet->_ipHeader->length - (packet->_ipHeader->len * 4 + packet->_tcpHeader->length * 4);

	CHAR tmp[128] = { 0 };

	auto old_uses = std::to_string(pkthdr->ts.tv_usec);
	// padding zero
	auto new_usec = std::string(6 - min(6, old_uses.length()), '0') + old_uses;

	sprintf_s(tmp, "%d%s", pkthdr->ts.tv_sec, new_usec.c_str());
	packet->_ts = atoll(tmp) / 1000;

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