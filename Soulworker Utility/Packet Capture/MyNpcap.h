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
public:
	MyNpcap() { }
	~MyNpcap() {
		_stopNpcap = true;
	}

	DWORD Init();
};

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* tcpReassemblyCookie);

static VOID tcpReassemblyMsgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData& tcpData, void* userCookie);
static VOID tcpReassemblyConnectionStartCallback(const pcpp::ConnectionData& connectionData, void* userCookie);
static VOID tcpReassemblyConnectionEndCallback(const pcpp::ConnectionData& connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void* userCookie);