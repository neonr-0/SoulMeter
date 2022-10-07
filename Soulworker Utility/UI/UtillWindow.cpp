#include "pch.h"
#include "UtillWindow.h"

VOID UtillWindow::OpenWindow()
{
	_isOpen = true;
}

VOID UtillWindow::handleDialogInfo()
{
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
						_currentIndex = -1;
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

VOID UtillWindow::HistoryWindow()
{
	if (ImGui::BeginTabItem(LANGMANAGER.GetText("STR_MENU_HISTORY")))
	{
		CHAR label[1024] = { 0 };

		ImGui::InputText(LANGMANAGER.GetText("STR_UTILLWINDOW_SEARCH"), _searchData, IM_ARRAYSIZE(_searchData));

		if (UIOPTION.isUseSaveData())
		{
			sprintf_s(label, "%s(%s %d) : %s", LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_USING"), LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_MAX"), HISTORY_SIZE, SAVEDATA.CurrentSaveDataPath().c_str());
		}
		else {
			sprintf_s(label, "%s(%s %d)", LANGMANAGER.GetText("STR_MENU_HISTORY"), LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_MAX"), HISTORY_SIZE);
		}
		ImGui::Text(label);

		if (UIOPTION.isUseSaveData())
		{
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

			handleDialogInfo();

			ImGui::SameLine();
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
		}

		ImGui::SameLine();
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

		ImGui::SameLine();
		if (ImGui::Button(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_DELETE_SELECTED")))
		{
			HISTORY.GetLock();
			{
				DAMAGEMETER.GetLock();
				{
					if (!DAMAGEMETER.isRun() && DAMAGEMETER.isHistoryMode() && DAMAGEMETER.GetCurrentHistoryId() >= 0)
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

		if (HISTORY.GetCurrentIndex() != _currentIndex && !HISTORY.isStop())
		{
			HISTORY.GetLock();
			{
				_currentIndex = HISTORY.GetCurrentIndex();
				DAMAGEMETER.SetCurrentHistoryId(-1);
				_historyTmp.clear();

				if (HISTORY.size() > 0) {
					INT32 i = static_cast<INT32>(HISTORY.size());
					for (auto itr = HISTORY.rbegin(); itr != HISTORY.rend(); itr++)
					{
						HISTORY_INFO* hi = (HISTORY_INFO*)*itr;

						CHAR mapName[MAX_MAP_LEN] = { 0 };
						SWDB.GetMapName(hi->_worldID, mapName, MAX_MAP_LEN);

						CHAR extInfo[256] = { 0 };
						//if (hi->_historyData->_extInfo.length() > 0)
						//	sprintf_s(extInfo, "(%s)", hi->_historyData->_extInfo.c_str());
#ifdef _DEBUG
						sprintf_s(extInfo, "(M:%u)", hi->_worldID);
#endif

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

						_historyTmp.push_back(std::pair(hi, string(label)));
					}
				}

				HISTORY.FreeLock();
			}
		}

		ImGui::BeginChild("select history", ImVec2(0, 0), true);
		{
			INT32 i = static_cast<INT32>(_historyTmp.size());
			for (auto itr = _historyTmp.begin(); itr != _historyTmp.end(); itr++)
			{
				if (itr->first == nullptr)
					continue;
				if (strlen(_searchData) > 0 && itr->second.find(string(_searchData)) == std::string::npos)
					continue;

				if (ImGui::Selectable(itr->second.c_str(), DAMAGEMETER.GetHistoryHI() == itr->first) && !DAMAGEMETER.isRun()) {
					if (!DAMAGEMETER.isRun()) {
						DAMAGEMETER.Clear();
						DAMAGEMETER.SetCurrentHistoryId(i);
						DAMAGEMETER.SetHistory((LPVOID)itr->first);
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
}

VOID UtillWindow::CombatWindow()
{
	if (ImGui::BeginTabItem(LANGMANAGER.GetText("STR_UTILLWINDOW_COMBAT")))
	{
		CHAR label[1024] = { 0 };
		_mutex.lock();
		{
			auto combatIF = COMBATMETER.Get();

			if (combatIF != nullptr)
			{
				if (_ci == nullptr)
				{
					COMBATMETER.GetLock();
					{
						if (combatIF->begin() != combatIF->end())
							_ci = combatIF->begin()->second;

						COMBATMETER.FreeLock();
					}
				}

				if (_ci != nullptr)
				{
					ImGui::InputText(LANGMANAGER.GetText("STR_UTILLWINDOW_SEARCH"), _searchData2, IM_ARRAYSIZE(_searchData2));

					sprintf_s(label, "%s###UtillCombatSelector", LANGMANAGER.GetText("STR_UTILLWINDOW_COMBAT_SELECTOR"));
					if (ImGui::BeginCombo(label, COMBATMETER.GetName(_ci).c_str(), ImGuiComboFlags_HeightLarge))
					{
						COMBATMETER.GetLock();
						{
							for (auto itr = combatIF->begin(); itr != combatIF->end(); itr++)
							{
								sprintf_s(label, "%s##%d", COMBATMETER.GetName(itr->second).c_str(), itr->first);

								if (ImGui::Selectable(label, _ci == itr->second)) {
									_ci = itr->second;
									ForceUpdateCombatTemp(_ci);
								}
							}
							COMBATMETER.FreeLock();
						}
						ImGui::EndCombo();
					}

					ImGui::BeginChild("display combat log", ImVec2(0, 0), true);
					{
						if (_ci->size() != _combatTmp.size())
						{
							COMBATMETER.GetLock();
							{
								ForceUpdateCombatTemp(_ci);
								COMBATMETER.FreeLock();
							}
						}

						for (auto itr = _combatTmp.begin(); itr != _combatTmp.end(); itr++)
						{
							const char* label = itr->second.c_str();

							if (strlen(_searchData2) > 0 && string(label).find(string(_searchData2)) == std::string::npos)
								continue;

							ImGui::Text(label);
						}
						ImGui::EndChild();
					}
				}
			}
			_mutex.unlock();
		}
		ImGui::EndTabItem();
	}
}

VOID UtillWindow::ForceUpdateCombatTemp(Combat* pCombat)
{
	_combatTmp.clear();

	CHAR label[1024] = { 0 };
	size_t i = pCombat->size();
	int j = 0;
	for (auto itr2 = pCombat->rbegin(); itr2 != pCombat->rend(); itr2++)
	{
		CombatLog* pCombatLog = itr2->second;
		SYSTEMTIME* time = pCombatLog->_time;

		sprintf_s(label, "%llu.[%02d:%02d:%02d.%03d] %s",
			i--,
			time->wHour, time->wMinute, time->wSecond, time->wMilliseconds,
			COMBATMETER.ConvertCombatLogVal(pCombatLog, (CombatType)pCombat->GetType()).c_str()
		);

		_combatTmp.push_back(std::pair(j++, string(label)));
	}
}

VOID UtillWindow::ClearCombatTemp()
{
	_mutex.lock();
	{
		_ci = nullptr;
		_combatTmp.clear();
		_mutex.unlock();
	}
}

VOID UtillWindow::Update()
{
	if (!_isOpen)
		return;

	CHAR label[1024] = { 0 };
	sprintf_s(label, "%s###UtillWindow", LANGMANAGER.GetText("STR_MENU_UTILL"));

	ImGui::Begin(label, &_isOpen, ImGuiWindowFlags_None);
	{
		if (ImGui::BeginTabBar(u8"UtillWindowTab"))
		{
			HistoryWindow();
			CombatWindow();
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

UtillWindow::UtillWindow()
{
}

UtillWindow::~UtillWindow()
{
	BOOL a = _mutex.try_lock();
	_mutex.unlock();
}
