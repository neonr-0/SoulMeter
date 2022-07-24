#include "pch.h"
#include "UtillWindow.h"
#include ".\Damage Meter\History.h"
#include ".\Damage Meter\MySQLite.h"


VOID UtillWindow::OpenWindow()
{
	_isOpen = true;
}

VOID UtillWindow::Update()
{

	if (!_isOpen)
		return;

	CHAR label[256] = { 0 };
	sprintf_s(label, "%s###UtillWindow", LANGMANAGER.GetText("STR_MENU_HISTORY"));

	ImGui::Begin(label, &_isOpen, ImGuiWindowFlags_None);
	{
		CHAR searchData[MAX_PATH] = { 0 };
		ImGui::InputText(LANGMANAGER.GetText("STR_UTILLWINDOW_SEARCH"), searchData, IM_ARRAYSIZE(searchData));

		CHAR label[MAX_PATH] = { 0 };
		sprintf_s(label, "%s(%s %d)", LANGMANAGER.GetText("STR_MENU_HISTORY"), LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_MAX"), HISTORY_SIZE);
		ImGui::Text(label);

		const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;
		static float ButtonWidth = 120.0f;
		float pos = ButtonWidth + ItemSpacing;
		ImGui::SameLine(ImGui::GetWindowWidth() - pos);
		if (ImGui::Button(LANGMANAGER.GetText("STR_UTILLWINDOW_HISTORY_DELETE_SELECTED")))
		{
			HISTORY.GetMutex()->lock();
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
				HISTORY.GetMutex()->unlock();
			}
		}

		ImGui::BeginChild("select history", ImVec2(0, 0), true);
		{
			auto vector = HISTORY.GetVector();

			if (HISTORY.GetCurrentIndex() != _currentIndex && !HISTORY.isStop()) {
				HISTORY.GetMutex()->lock();

				if (vector->size() >= 0) {
					_hi = *vector;
					_currentIndex = HISTORY.GetCurrentIndex();
				}
				DAMAGEMETER.SetCurrentHistoryId(-1);

				HISTORY.GetMutex()->unlock();
			}

			if (_hi.size() > 0) {
				INT32 i = 1;
				for (auto itr = _hi.rbegin(); itr != _hi.rend(); itr++)
				{
					HISTORY_INFO* hi = (HISTORY_INFO*)*itr;

					CHAR mapName[MAX_MAP_LEN] = { 0 };
					SWDB.GetMapName(hi->_worldID, mapName, MAX_MAP_LEN);

					CHAR extInfo[128] = { 0 };
					if (hi->_historyData->_extInfo.length() > 0)
						sprintf_s(extInfo, "(%s)", hi->_historyData->_extInfo.c_str());

					sprintf_s(label, "%d.[%02d/%02d %02d:%02d:%02d] %s%s - %02d:%02d.%01d###history%d", 
						i,
						hi->_saveTime->wMonth, hi->_saveTime->wDay,hi->_saveTime->wHour, hi->_saveTime->wMinute, hi->_saveTime->wSecond,
						extInfo,
						mapName,
						(UINT)hi->_time / (60 * 1000), (UINT)(hi->_time / 1000) % 60, (UINT)hi->_time % 1000 / 100,
						i
					);

					if (strlen(searchData) > 0 && string(label).find(string(searchData)) == std::string::npos)
						continue;

					if (ImGui::Selectable(label, DAMAGEMETER.GetCurrentHistoryId() == i) && !DAMAGEMETER.isRun()) {
						if (!DAMAGEMETER.isRun()) {
							DAMAGEMETER.SetCurrentHistoryId(i);
							DAMAGEMETER.SetHistory((LPVOID)hi);
						}
					}
					i++;
				}
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
