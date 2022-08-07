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

	UINT64 _allLoss = 0;
	UINT64 _loss = 0;
	UINT64 _prevLossTime = 0;
	mutex _lossMutex;

	BOOL _stopCapture = FALSE;

public:
	PacketCapture() {}
	~PacketCapture() {
		_stopCapture = TRUE;
		BOOL a = _lossMutex.try_lock();
		_lossMutex.unlock();
	}

	BOOL Init();
	static VOID ParseWinDivertStruct(IPv4Packet* packet, uint8_t* pkt);

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

	const UINT64& GetAllLoss()
	{
		return _allLoss;
	}

	VOID UpdateLoss(BOOL add)
	{
		if (_stopCapture)
			return;

		_lossMutex.lock();
		{
			ULONG64 time = GetCurrentTimeStamp();
			// reset loss
			if (_prevLossTime < time || _loss == 0)
			{
				_loss = 0;
				// 10s
				_prevLossTime = time + 10000;
			}

			if (add)
			{
				_loss++;
				_allLoss++;
			}

			_lossMutex.unlock();
		}
	}
};