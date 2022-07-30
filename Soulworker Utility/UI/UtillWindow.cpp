#include "pch.h"
#include "UtillWindow.h"
#include ".\Damage Meter\History.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\Damage Meter\SaveData.h"
#include ".\UI\UiWindow.h"

VOID UtillWindow::OpenWindow()
{
	_isOpen = true;
}

VOID UtillWindow::Update()
{
	if (!_isOpen)
		return;

	CHAR label[1024] = { 0 };
	sprintf_s(label, "%s###UtillWindow", LANGMANAGER.GetText("STR_MENU_HISTORY"));

	ImGui::Begin(label, &_isOpen, ImGuiWindowFlags_None);
	{
		ImGui::InputText(LANGMANAGER.GetText("STR_UTILLWINDOW_SEARCH"), _searchData, IM_ARRAYSIZE(_searchData));

		if (UIOPTION.isUseSaveData())
		{
			sprintf_s(label, "%s : %s", LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_USING"), SAVEDATA.CurrentSaveDataPath().c_str());
			ImGui::Text(label);
		}

		sprintf_s(label, "%s(%s %d)", LANGMANAGER.GetText("STR_MENU_HISTORY"), LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_MAX"), HISTORY_SIZE);
		ImGui::Text(label);

		const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;
		static float ButtonWidth = 120.0f;
		float pos = ImGui::GetWindowWidth() - (ButtonWidth + ItemSpacing);
		ImGui::SameLine(pos);
		if (ImGui::Button(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_DELETE_SELECTED")))
		{
			HISTORY.GetLock();
			{
				DAMAGEMETER.GetLock();
				{
					if (!DAMAGEMETER.isRun() && DAMAGEMETER.isHistoryMode() && DAMAGEMETER.GetCurrentHistoryId() > 0)
					{
						HISTORY_INFO* HI = (HISTORY_INFO*)DAMAGEMETER.GetHistoryHI();
						DAMAGEMETER.Clear();

						HISTORY.ClearHistory(HI, FALSE);
						_currentIndex = -1;
					}
					DAMAGEMETER.FreeLock();
				}
				HISTORY.FreeLock();
			}
		}

		pos -= 125;
		ImGui::SameLine(pos);
		if (ImGui::Button(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_CLEARALL")))
		{
			HISTORY.GetLock();
			{
				DAMAGEMETER.GetLock();
				{
					ANSItoUTF8(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_CLEARALL_CONFIRM"), label, sizeof(label));
					if (!DAMAGEMETER.isRun() && HISTORY.size() > 0 && MessageBoxA(UIWINDOW.GetHWND(), label, "WARNING", MB_ICONWARNING | MB_YESNO | MB_TOPMOST) == IDYES)
					{
						DAMAGEMETER.Clear();
						HISTORY.ClearAll();
						_currentIndex = -1;
					}
					DAMAGEMETER.FreeLock();
				}
				HISTORY.FreeLock();
			}
		}


		if (UIOPTION.isUseSaveData())
		{
			pos -= 125;
			ImGui::SameLine(pos);
			if (ImGui::Button(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_SAVETO")))
			{
				if (!DAMAGEMETER.isRun() && HISTORY.size() > 0)
				{
					_fileDialogOpen = true;
					_fileDialogInfo.type = ImGuiFileDialogType_SaveFile;
					_fileDialogInfo.title = "Save SoulMeter savedata###FileDiglogSave";
					_fileDialogInfo.fileName = ".dat";
					_fileDialogInfo.directoryPath = std::filesystem::current_path();
				}
			}

			pos -= 125;
			ImGui::SameLine(pos);
			if (ImGui::Button(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_LOAD")))
			{
				if (!DAMAGEMETER.isRun())
				{
					_fileDialogOpen = true;
					_fileDialogInfo.type = ImGuiFileDialogType_OpenFile;
					_fileDialogInfo.title = "Load SoulMeter savedata###FileDiglogLoad";
					_fileDialogInfo.fileName = ".dat";
					_fileDialogInfo.directoryPath = std::filesystem::current_path();
				}
			}

			if (ImGui::FileDialog(&_fileDialogOpen, &_fileDialogInfo))
			{
				HISTORY.GetLock();
				{
					DAMAGEMETER.GetLock();
					{
						if (!DAMAGEMETER.isRun())
						{
							DAMAGEMETER.Clear();
							if (_fileDialogInfo.type == ImGuiFileDialogType_SaveFile)
							{
								if (HISTORY.size() > 0)
									SAVEDATA.Clone(_fileDialogInfo.resultPath.generic_string());
							}
							else if (_fileDialogInfo.type == ImGuiFileDialogType_OpenFile)
							{
								HISTORY.ClearVector();
								SAVEDATA.GetLock();
								{
									SAVEDATA.Reset();
									if (SAVEDATA.Init(_fileDialogInfo.resultPath.generic_string()))
									{
										SAVEDATA.Reset();
										if (SAVEDATA.Init())
										{
											Log::WriteLogA("[UtillWindow::Update] Load savedata failed");
											exit(1);
										}
										CHAR label[256] = { 0 };
										ANSItoUTF8(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_LOAD_FAILED"), label, sizeof(label));
										MessageBoxA(UIWINDOW.GetHWND(), label, "ERROR", MB_ICONERROR | MB_OK);
									}
									SAVEDATA.FreeLock();
								}
							}
						}
						DAMAGEMETER.FreeLock();
					}
					HISTORY.FreeLock();
				}
			}
		}

		ImGui::BeginChild("select history", ImVec2(0, 0), true);
		{
			HISTORY.GetLock();
			{
				if (HISTORY.GetCurrentIndex() != _currentIndex && !HISTORY.isStop()) {
					_currentIndex = HISTORY.GetCurrentIndex();
					DAMAGEMETER.SetCurrentHistoryId(-1);
				}

				if (HISTORY.size() > 0) {
					INT32 i = static_cast<INT32>(HISTORY.size());
					for (auto itr = HISTORY.rbegin(); itr != HISTORY.rend(); itr++)
					{
						HISTORY_INFO* hi = (HISTORY_INFO*)*itr;

						CHAR mapName[MAX_MAP_LEN] = { 0 };
						SWDB.GetMapName(hi->_worldID, mapName, MAX_MAP_LEN);

						CHAR extInfo[256] = { 0 };
						if (hi->_historyData->_extInfo.length() > 0)
							sprintf_s(extInfo, "(%s)", hi->_historyData->_extInfo.c_str());

						CHAR realClearTime[128] = { 0 };
						if (hi->_realClearTime > 0)
						{
							sprintf_s(realClearTime, "(%02d:%02d.%01d)",
								(UINT)hi->_realClearTime / 10 % 3600 / 60, (UINT)hi->_realClearTime / 10 % 3600 % 60, (UINT)hi->_realClearTime % 10
							);
						}

						sprintf_s(label, "%d.[%02d/%02d %02d:%02d:%02d] %s%s - %02d:%02d.%01d%s###history%d",
							i,
							hi->_saveTime->wMonth, hi->_saveTime->wDay, hi->_saveTime->wHour, hi->_saveTime->wMinute, hi->_saveTime->wSecond,
							extInfo,
							mapName,
							(UINT)hi->_time / (60 * 1000), (UINT)(hi->_time / 1000) % 60, (UINT)hi->_time % 1000 / 100,
							realClearTime,
							i
						);

						i--;

						if (strlen(_searchData) > 0 && string(label).find(string(_searchData)) == std::string::npos)
							continue;

						if (ImGui::Selectable(label, DAMAGEMETER.GetCurrentHistoryId() == i) && !DAMAGEMETER.isRun()) {
							if (!DAMAGEMETER.isRun()) {
								DAMAGEMETER.SetCurrentHistoryId(i);
								DAMAGEMETER.SetHistory((LPVOID)hi);
							}
						}
					}
				}
				HISTORY.FreeLock();
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

UtillWindow::UtillWindow()
{
}

UtillWindow::~UtillWindow()
{
}
