#pragma once

#include "pch.h"

#include ".\Third Party\PcapPlusPlus\header\TcpReassembly.h"
#include ".\Third Party\PcapPlusPlus\header\PcapLiveDeviceList.h"
#include ".\Third Party\PcapPlusPlus\header\PcapLiveDevice.h"
#include ".\Third Party\PcapPlusPlus\header\SystemUtils.h"
#include ".\Third Party\PcapPlusPlus\header\PcapPlusPlusVersion.h"
#include ".\Third Party\PcapPlusPlus\header\LRUList.h"
#include ".\Third Party\PcapPlusPlus\header\TCPLayer.h"
using namespace std;

#define NPCAP MyNpcap::getInstance()

// https://npcap.com/guide/wpcap/pcap-filter.html
#define NPCAP_FILTER_RULE "ip and (src port 10200 or dst port 10200)"

#if _DEBUG
#define DEBUG_NPCAP_SORT 0
#endif

static BOOL _stopNpcap = FALSE;

struct CaptureInfo
{
	pcpp::TcpReassembly* recvReassembly;
	pcpp::TcpReassembly* sendReassembly;
};

class MyNpcap : public Singleton<MyNpcap> {
private:
	struct ThreadInfo
	{
		MyNpcap* _this;
		pcpp::PcapLiveDevice* dev;
		std::string bpfFilter;
	};

	DWORD LoadNpcapDlls();

	VOID sniffAllInterface(string bpfFilter);

	static DWORD doTcpReassemblyOnLiveTraffic(LPVOID param);

	BOOL _inited = FALSE;

	VOID StopSniffAllInterface();

	mutex _recv;
	mutex _send;

	vector<IPv4Packet*> _recvQueue;
	vector<IPv4Packet*> _sendQueue;

	static DWORD processQueuePacket(LPVOID param);
public:
	MyNpcap() { }
	~MyNpcap() {
		_stopNpcap = true;
		BOOL a = _recv.try_lock();
		a = _send.try_lock();
		_recv.unlock();
		_send.unlock();
	}

	DWORD Init();

	VOID GetLock(BOOL isRecv)
	{
		if (_stopNpcap)
			return;

		if (isRecv)
			_recv.lock();
		else
			_send.lock();
	}

	VOID FreeLock(BOOL isRecv)
	{
		if (_stopNpcap)
			return;

		if (isRecv)
			_recv.unlock();
		else
			_send.unlock();
	}

	VOID push_back(BOOL isRecv, IPv4Packet* pPacket)
	{
		if (_stopNpcap)
			return;

		if (isRecv)
			_recvQueue.push_back(pPacket);
		else
			_sendQueue.push_back(pPacket);
	}

	auto begin(BOOL isRecv)
	{
		if (isRecv)
			return _recvQueue.begin();
		else
			return _sendQueue.begin();
	}

	auto end(BOOL isRecv)
	{
		if (isRecv)
			return _recvQueue.end();
		else
			return _sendQueue.end();
	}

	VOID erase(BOOL isRecv, vector<IPv4Packet*>::iterator itr)
	{
		if (isRecv)
			_recvQueue.erase(itr);
		else
			_sendQueue.erase(itr);
	}

	BOOL empty(BOOL isRecv)
	{
		if (isRecv)
			return _recvQueue.empty();
		else
			return _sendQueue.empty();
	}
};

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* tcpReassemblyCookie);

static VOID tcpReassemblyMsgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData& tcpData, void* userCookie);
static VOID tcpReassemblyConnectionStartCallback(const pcpp::ConnectionData& connectionData, void* userCookie);
static VOID tcpReassemblyConnectionEndCallback(const pcpp::ConnectionData& connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void* userCookie);