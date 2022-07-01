#pragma once

#include "pch.h"
using namespace std;

#define NPCAP MyNpcap::getInstance()

// https://npcap.com/guide/wpcap/pcap-filter.html
#define NPCAP_FILTER_RULE "ip and (src port 10200 or dst port 10200)"

#if _DEBUG
#define DEBUG_NPCAP_SORT 0
#endif

class MyNpcap : public Singleton<MyNpcap> {
private:

	struct ThreadInfo
	{
		pcap_t* _device;
		MyNpcap* _thisl;
	};

	DWORD LoadNpcapDlls();
	DWORD Filter(pcap_t* device);

	static DWORD WINAPI CreatePcapLoop(LPVOID pDevice);
	static VOID ReceiveCallback(u_char* prc, const struct pcap_pkthdr* header, const u_char* pkt_data);

	HANDLE _handle;

public:
	MyNpcap() { }
	~MyNpcap() {}

	DWORD Init();
};