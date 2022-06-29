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
#define DEBUG_CAPTURE_SORT 0
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

	recursive_mutex _recvMutex;
	recursive_mutex _sendMutex;

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
	}

	BOOL Init();
	static VOID ParseWinDivertStruct(IPv4Packet* packet, BYTE* pkt);
	static VOID ParseNpcapStruct(IPv4Packet* packet, BYTE* pkt, pcap_pkthdr* pkthdr);

	static VOID ClearPacketInfo(PacketInfo* pi);

	CHAR* GetType()
	{
		if (_useType == CaptureType::_NPCAP)
			return "Npcap";
		else
			return "Windivert";
	}

	VOID InsertQueue(ULONG seq, PacketInfo* pi, BOOL isRecv)
	{
		if (isRecv) {
			_recvMutex.lock();
			_recvPacketQueue[seq] = pi;
			_recvMutex.unlock();
		}
		else {
			_sendMutex.lock();
			_sendPacketQueue[seq] = pi;
			_sendMutex.unlock();
		}
	}

	BOOL QueueIsEmpty(BOOL isRecv)
	{
		if (isRecv)
			return _recvPacketQueue.empty();
		else
			return _sendPacketQueue.empty();
	}

	recursive_mutex* GetRecvMutex()
	{
		return &_recvMutex;
	}

	recursive_mutex* GetSendMutex()
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