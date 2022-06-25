#pragma once

#include "pch.h"
#include <queue>
using namespace std;

#define WINDIVERT MyWinDivert::getInstance()

#define WINDIVERT_FILTER_RULE "(tcp.SrcPort == 10200 or tcp.DstPort == 10200) and tcp.PayloadLength > 0 and ip"

//	 Windivert 1.4.2
#define WINDIVERT_MTU_MAX (40 + 0xFFFF)

#ifdef _DEBUG
#define DEBUG_DIVERT_ALL 0
#define DEBUG_DIVERT_IP 0
#define DEBUG_DIVERT_TCP 0
#define DEBUG_DIVERT_DATA 0
#endif

class MyWinDivert : public Singleton<MyWinDivert> {
private:
	struct PacketInfo
	{
		BYTE* _pkt;
		MyWinDivert* _this;
	};

	static DWORD WINAPI ReceiveCallback(LPVOID prc);
	static DWORD WINAPI ParsePacket(LPVOID prc);

	HANDLE _handle;

	static VOID PrintIPHeader(IPv4Packet* p_packet);
	static VOID PrintTCPHeader(IPv4Packet* p_packet);

	queue<PacketInfo*> _packetQueue;

public:
	MyWinDivert() { }
	~MyWinDivert() {}

	DWORD Init();
	static IPv4Packet* ParseStruct(BYTE* pkt);
};