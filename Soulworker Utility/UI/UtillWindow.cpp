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
		ImGui::BeginChild("select history", ImVec2(0, 0), true);
		{
			auto vector = HISTORY.GetVector();

			if (HISTORY.GetCurrentIndex() != _currentIndex && !HISTORY.isStop()) {
				HISTORY.GetMutex()->lock();

				if (vector->size() > 0) {
					_hi = *vector;
					_currentIndex = HISTORY.GetCurrentIndex();
				}
				DAMAGEMETER.SetCurrentHistoryId(-1);

				HISTORY.GetMutex()->unlock();
			}

			if (_hi.size() > 0) {
				INT32 i = 0;
				for (auto itr = _hi.rbegin(); itr != _hi.rend(); itr++)
				{
					HISTORY_INFO* hi = (HISTORY_INFO*)*itr;

					CHAR mapName[MAX_MAP_LEN] = { 0 };
					SWDB.GetMapName(hi->_worldID, mapName, MAX_MAP_LEN);

					sprintf_s(label, "%s - %02d:%02d.%01d (%02d:%02d:%02d)###history%d", 
						mapName,
						(UINT)hi->_time / (60 * 1000), (UINT)(hi->_time / 1000) % 60, (UINT)hi->_time % 1000 / 100,
						hi->_saveTime.wHour, hi->_saveTime.wMinute, hi->_saveTime.wSecond,
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
