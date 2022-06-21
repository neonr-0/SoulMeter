#pragma once

#include "pch.h"
using namespace std;

#define WINDIVERT MyWinDivert::getInstance()

#define WINDIVERT_FILTER_RULE "(tcp.SrcPort == 10200 or tcp.DstPort == 10200) and tcp.PayloadLength > 0 and ip"

#ifdef _DEBUG
#define DEBUG_DIVERT_ALL 0
#define DEBUG_DIVERT_IP 0
#define DEBUG_DIVERT_TCP 0
#define DEBUG_DIVERT_DATA 1
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

	mutex _mutex;

public:
	MyWinDivert() { }
	~MyWinDivert() {}

	DWORD Init();
};