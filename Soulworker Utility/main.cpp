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
			sprintf_s(errorMsg, "Init Lang failed. err: %lu", errorCode);
			Log::WriteLogA(errorMsg);
			break;
		}

		PLAYERTABLE.CheckUpdate();

		if ((errorCode = SWCRYPT.LoadSWCrypt())) {
			sprintf_s(errorMsg, "Load SWCrypt.dll failed, err: %lu", errorCode);
			Log::WriteLogA(errorMsg);
			break;
		}

		if (!SWDB.Init()) {
			sprintf_s(errorMsg, "Init database failed.");
			Log::WriteLogA(errorMsg);
			break;
		}

		if (UIWINDOW.Init(1, 1, 1, 1)) {
			if ((errorCode = PACKETCAPTURE.Init())) {
				sprintf_s(errorMsg, "Init PacketCapture failed, err: %lu", errorCode);
				Log::WriteLogA(errorMsg);
				break;
			}
			UIWINDOW.Run();
		}
		else {
			sprintf_s(errorMsg, "Init UI failed.");
			Log::WriteLogA(errorMsg);
			break;
		}

	} while (false);

	if (errorCode) {
		MessageBoxA(0, errorMsg, "SoulMeter", MB_ICONERROR | MB_OK);
	}

	MiniDump::End();
}