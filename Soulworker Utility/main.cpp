#include "pch.h"
#include ".\Packet Capture/PacketCapture.h"
#include ".\Damage Meter/Damage Meter.h"
#include ".\UI\UiWindow.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\UI\PlayerTable.h"
#include ".\SWCrypt\SWCryptDLL.h"
#include ".\Damage Meter\SaveData.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Packet Capture\MyNpcap.h"


#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )
#endif
int DoMeterUpdate();

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR szCmdLine, _In_ int iCmdShow) {

	MiniDump::Begin();
	{
		DWORD errorCode = ERROR_SUCCESS;
		CHAR errorMsg[512] = { 0 };

		// Parsing input arguments
		int argc = 0;
		wchar_t** argv = CommandLineToArgvW(szCmdLine, &argc);
		if (argc >= 1)
		{
			bool updateNeed = false;
			bool updateDatabase = false;
			for (int i=0; i < argc; i++)
			{
				if (_wcsicmp(argv[i], L"-update") == NULL)
					updateNeed = true;
				if (_wcsicmp(argv[i], L"-updatedatabase") == NULL)
					updateDatabase = true;
			}
			if (updateNeed)
			{
				errorCode = DoMeterUpdate();
				if (errorCode) 
					sprintf_s(errorMsg, "Update failed: %lu", errorCode);
				
				MiniDump::End();
				exit(0);
			}
		}
		// Cleanup if update was happen
		if (std::filesystem::exists("./tmp/"))
		{
			std::filesystem::remove_all("./tmp/");
			std::filesystem::remove("./update.exe");
		}
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
				break;
			}

			if ((errorCode = SWCRYPT.LoadSWCrypt())) {
				sprintf_s(errorMsg, "Load SWCrypt.dll failed, err: %lu", errorCode);
				break;
			}

			if (!SWDB.Init()) {
				sprintf_s(errorMsg, "Init database failed.");
				break;
			}

			if (UIWINDOW.Init(1, 1, 1, 1)) {
				if ((errorCode = PACKETCAPTURE.Init())) {
					sprintf_s(errorMsg, "Init PacketCapture failed, err: %lu", errorCode);
					break;
				}
				if (UIOPTION.isUseSaveData())
				{
					if ((errorCode = SAVEDATA.Init())) {
						sprintf_s(errorMsg, "Init SaveData failed, err: %lu", errorCode);
						if (errorCode == ERROR_FILE_CORRUPT) {
							ANSItoUTF8(LANGMANAGER.GetText("STR_SAVEDATA_VERSION_ERROR"), errorMsg, sizeof(errorMsg));
						}
						else if (errorCode == ERROR_FILE_SYSTEM_LIMITATION) {
							ANSItoUTF8(LANGMANAGER.GetText("STR_SAVEDATA_MULTIPLE_ERROR"), errorMsg, sizeof(errorMsg));
						}
						break;
					}
				}
				if (UIOPTION.isUpdateCheck())
					PLAYERTABLE.CheckUpdate();
				UIWINDOW.Run();
			}
			else {
				sprintf_s(errorMsg, "Init UI failed.");
				break;
			}

		} while (false);

		if (errorCode != ERROR_SUCCESS) {
			MessageBoxA(NULL, errorMsg, "SoulMeter", MB_ICONERROR | MB_OK | MB_TOPMOST);
			Log::WriteLogA(errorMsg);
		}

		MiniDump::End();
	}

	ShowWindow(UIWINDOW.GetHWND(), 0);
	NPCAP.StopSniffAllInterface();
}
int DoMeterUpdate() 
{
	for (auto& p : std::filesystem::directory_iterator("tmp"))
	{
		try
		{
			if(!std::filesystem::is_directory(p))
				std::filesystem::copy(p, L"./", std::filesystem::copy_options::overwrite_existing); //file copy
			else
			{
				auto fpath = std::filesystem::path(p);
				wstring dstpath = L"./" + fpath.filename().wstring() + L"/";
				std::filesystem::copy(p, dstpath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive); //folder copy
			}
		}
		catch (const std::exception&)
		{
			//ignore
		}
	}
	wchar_t wpath[2048] = {};
	GetCurrentDirectoryW(2048, wpath);

	HINSTANCE hInst = ShellExecuteW(NULL, L"open", L"SoulMeter.exe", NULL, wpath, 1);
	return ERROR_SUCCESS;
}

