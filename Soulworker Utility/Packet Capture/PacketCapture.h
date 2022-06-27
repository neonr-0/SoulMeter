#pragma once

#include "pch.h"
#include <queue>
using namespace std;

#define PACKETCAPTURE PacketCapture::getInstance()


#ifdef _DEBUG
#define DEBUG_CAPTURE_ALL 0
#define DEBUG_CAPTURE_IP 0
#define DEBUG_CAPTURE_TCP 0
#define DEBUG_CAPTURE_DATA 0
#define DEBUG_CAPTURE_SORT 1
#endif

struct PacketInfo
{
	IPv4Packet* _packet;
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

	unordered_map<ULONG, PacketInfo*> _recvPacketQueue;
	unordered_map<ULONG, PacketInfo*> _sendPacketQueue;

	ULONG _nextRecvSEQ;
	ULONG _nextSendSEQ;

	static DWORD WINAPI PacketRoute(LPVOID prc);

	static DWORD WINAPI ParsePacket(LPVOID prc, PacketInfo* pi);

	HANDLE _handle;

	static VOID PrintIPHeader(IPv4Packet* p_packet);
	static VOID PrintTCPHeader(IPv4Packet* p_packet);

	CaptureType _useType;

	mutex _recvMutex;
	mutex _sendMutex;

	mutex _recvQueueMutex;
	mutex _sendQueueMutex;
	BOOL _stopCapture;

	BOOL _isInitRecv;
	BOOL _isInitSend;
	BOOL _pauseParse;

public:
	PacketCapture(): _isInitRecv(FALSE), _isInitSend(FALSE), _pauseParse(FALSE), _stopCapture(FALSE) {}
	~PacketCapture() {
		_pauseParse = TRUE;
		_stopCapture = TRUE;
		_recvMutex.lock();
		_recvMutex.unlock();
		_sendMutex.lock();
		_sendMutex.unlock();
		_recvQueueMutex.lock();
		_recvQueueMutex.unlock();
		_sendQueueMutex.lock();
		_sendQueueMutex.unlock();
	}

	BOOL Init();
	static VOID ParseWinDivertStruct(IPv4Packet* packet, BYTE* pkt);
	static VOID ParseNpcapStruct(IPv4Packet* packet, BYTE* pkt, UINT32 capLen);

	static VOID ClearPacketInfo(PacketInfo* pi);

	CHAR* GetType()
	{
		if (_useType == CaptureType::_NPCAP)
			return "Npcap";
		else
			return "Windivert";
	}

	unordered_map<ULONG, PacketInfo*>* GetRecvQueue()
	{
		return &_recvPacketQueue;
	}

	unordered_map<ULONG, PacketInfo*>* GetSendQueue()
	{
		return &_sendPacketQueue;
	}

	mutex* GetRecvQueueMutex()
	{
		return &_recvQueueMutex;
	}

	mutex* GetSendQueueMutex()
	{
		return &_sendQueueMutex;
	}

	mutex* GetRecvMutex()
	{
		return &_recvMutex;
	}

	mutex* GetSendMutex()
	{
		return &_sendMutex;
	}

	VOID ClearQueue(BOOL isRecv);

	VOID SetSEQ(ULONG l, BOOL isRecv)
	{
		if (isRecv) {
			_nextRecvSEQ = l;
			if (!_isInitRecv)
				_isInitRecv = TRUE;
		}
		else {
			_nextSendSEQ = l;
			if (!_isInitSend)
				_isInitSend = TRUE;
		}

#if DEBUG_CAPTURE_SORT == 1
		Log::WriteLogA("[DEBUG_CAPTURE_SORT] [%s] Set new SEQ %lu", isRecv ? "RECV" : "SEND", isRecv ? _nextRecvSEQ : _nextSendSEQ);
#endif
	}

	BOOL isInitRecv()
	{
		return _isInitRecv;
	}

	BOOL isInitSend()
	{
		return _isInitSend;
	}

	VOID SetPause(BOOL pause)
	{
		_pauseParse = pause;
	}

	BOOL isStopCaptue()
	{
		return _stopCapture;
	}
};