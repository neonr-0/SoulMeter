#include "pch.h"
#include ".\Packet Capture/PacketCapture.h"
#include ".\Damage Meter/Damage Meter.h"
#include ".\UI\UiWindow.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\UI\PlayerTable.h"
#include ".\SWCrypt\SWCryptDLL.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )
#endif

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR szCmdLine, _In_ int iCmdShow) {

	MiniDump::Begin();

	DWORD errorCode = 0;
	CHAR errorMsg[128] = { 0 };

	do
	{

		const UINT codePage = GetACP();
		switch (codePage) {
		case 936: // ZH-CN
		case 950: // ZH-TW
			_wsetlocale(LC_ALL, L"zh-TW.UTF8");
			errorCode = LANGMANAGER.SetCurrentLang("zh_tw.json");
			break;
		default:
			_wsetlocale(LC_ALL, L"en-US.UTF8");
			errorCode = LANGMANAGER.SetCurrentLang("en.json");
			break;
		}

		if (errorCode) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Init lang file failed. %d")), errorCode);
			sprintf_s(errorMsg, "Init Lang failed.");
			break;
		}

		PLAYERTABLE.CheckUpdate();

		if ((errorCode = SWCRYPT.LoadSWCrypt())) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Load SWCrypt.dll Failed %d")), errorCode);
			sprintf_s(errorMsg, "Load SWCrypt.dll failed.");
			break;
		}

		if (!SWDB.Init()) {
			Log::WriteLog(const_cast<LPTSTR>(_T("InitDB Failed")));
			sprintf_s(errorMsg, "Init database failed.");
			break;
		}

		if ((errorCode = PACKETCAPTURE.Init())) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Init PacketCapture Failed %d")), errorCode);
			sprintf_s(errorMsg, "Init PacketCapture failed.");
			break;
		}

		if (UIWINDOW.Init(1, 1, 1, 1)) {
			UIWINDOW.Run();
		}
		else {
			Log::WriteLog(const_cast<LPTSTR>(_T("Init UIWINDOW Failed")));
			sprintf_s(errorMsg, "Init UI failed.");
			break;
		}

	} while (false);

	if (errorCode) {
		MessageBoxA(0, errorMsg, "SoulMeter", MB_ICONERROR | MB_OK);
		exit(1);
	}

	MiniDump::End();
}