#include "pch.h"
#include ".\Packet Capture/PacketCapture.h"
#include ".\Damage Meter/Damage Meter.h"
#include ".\UI\UiWindow.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\UI\PlayerTable.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )
#endif

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR szCmdLine, _In_ int iCmdShow) {

	MiniDump::Begin();

	DWORD errorCode = 0;
	const UINT codePage = GetACP();
	switch (codePage) {
	case 936: // ZH-CN
	case 950: // ZH-TW
		_wsetlocale(LC_ALL, L"zh-TW.UTF8");
		LANGMANAGER.SetCurrentLang(ZH_TW);
		break;
	default:
		_wsetlocale(LC_ALL, L"en-US.UTF8");
		LANGMANAGER.SetCurrentLang(EN_US);
		break;
	}

	PLAYERTABLE.CheckUpdate();

	if (SWCRYPT.LoadSWCrypt()) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Load SWCrypt.dll Failed")));
		exit(-1);
	}

	if (!SWDB.Init()) {
		Log::WriteLog(const_cast<LPTSTR>(_T("InitDB Failed")));
		exit(-1);
	}

	if ((errorCode = PACKETCAPTURE.Init())) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Init PacketCapture Failed %d")), errorCode);
		exit(-1);
	}

	if (UIWINDOW.Init(1, 1, 1, 1)) {
		UIWINDOW.Run();
	}
	else {
		Log::WriteLog(const_cast<LPTSTR>(_T("Init UIWINDOW Failed")));
	}

	MiniDump::End();
}