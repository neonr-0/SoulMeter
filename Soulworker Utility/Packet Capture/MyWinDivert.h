#pragma once

#include "pch.h"
using namespace std;

#define WINDIVERT MyWinDivert::getInstance()

#define WINDIVERT_FILTER_RULE "(tcp.SrcPort == 10200 or tcp.DstPort == 10200) and tcp.PayloadLength > 0 and ip"

//	 Windivert 1.4.2
#define WINDIVERT_MTU_MAX (40 + 0xFFFF)

class MyWinDivert : public Singleton<MyWinDivert> {
private:

	static DWORD WINAPI ReceiveCallback(LPVOID prc);

	HANDLE _handle;

	BOOL _stop = FALSE;
	BOOL _inited = FALSE;

public:
	MyWinDivert() { }
	~MyWinDivert() {}

	DWORD Init(HANDLE handle);
};