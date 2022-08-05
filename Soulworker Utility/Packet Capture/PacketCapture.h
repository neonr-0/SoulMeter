#pragma once

#include "pch.h"
#include <queue>
#include ".\UI\Option.h"
using namespace std;

#define PACKETCAPTURE PacketCapture::getInstance()

#ifdef _DEBUG
#define DEBUG_CAPTURE_ALL 0
#define DEBUG_CAPTURE_IP 0
#define DEBUG_CAPTURE_TCP 0
#define DEBUG_CAPTURE_DATA 0
#define DEBUG_CAPTURE_SORT 0
#define DEBUG_CAPTURE_QUEUE 0
#endif

struct PacketInfo
{
	IPv4Packet* _packet;
	UINT64 _ts;
};

enum class CaptureType
{
	_NPCAP,
	_WINDIVERT
};

class PacketCapture : public Singleton<PacketCapture> {
private:
	struct ParseThreadInfo
	{
		PacketCapture* _this;
		BOOL isRecv;
	};

	static DWORD WINAPI PacketRoute(LPVOID prc);

	static DWORD WINAPI ParsePacket(LPVOID prc, PacketInfo* pi);

	HANDLE _handle;

	static VOID PrintIPHeader(IPv4Packet* p_packet);
	static VOID PrintTCPHeader(IPv4Packet* p_packet);
	
	INT32 _useMode;

	UINT64 _loss = 0;

public:
	PacketCapture() {}
	~PacketCapture() {}

	BOOL Init();
	static VOID ParseWinDivertStruct(IPv4Packet* packet, BYTE* pkt);

	static VOID ClearPacketInfo(PacketInfo* pi);

	CHAR* GetType(INT32* type = nullptr)
	{
		if (type == nullptr)
			type = &_useMode;

		if (*type == (INT32)CaptureType::_NPCAP)
			return "Npcap";
		else
			return "Windivert";
	}

	const INT32& GetMode()
	{
		return _useMode;
	}

	const UINT64& GetLoss()
	{
		return _loss;
	}
};