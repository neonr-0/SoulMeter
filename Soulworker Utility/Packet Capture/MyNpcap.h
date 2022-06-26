#pragma once

#include "pch.h"
using namespace std;

#define NPCAP MyNpcap::getInstance()

// https://npcap.com/guide/wpcap/pcap-filter.html
#define NPCAP_FILTER_RULE "ip and (src port 10200 or dst port 10200)"

class MyNpcap : public Singleton<MyNpcap> {
private:

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