#include "pch.h"
#include "PlotWindow.h"
#include "UtillWindow.h"
#include <vector>
#include ".\Language\Region.h"

VOID PlotWindow::AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement)
{
	if (isFirstElement) {
		if (_lastTime == time) {
			_allowed = false;
			return;
		}
		else {
			_allowed = true;
		}
	}
	// 첫번째가 스킵되면 나머지도 같이 스킵되어야함
	else {
		if (!_allowed) {
			return;
		}
	}
	_lastTime = time;

	auto it = metaInfos.begin();
	bool metaInfoFound = false;
	for (; it != metaInfos.end(); it++) {
		if ((*it)->_id == id) {
			metaInfoFound = true;
			break;
		}
	}

	if (!metaInfoFound) {
		metaInfos.push_back(new metaInfo(id, name));

		vector<double> newDPSvector;
		newDPSvector.push_back(DPS);
		dpsList.emplace(id, newDPSvector);

		vector<double> newTimevector;
		newTimevector.push_back(time);
		timeList.emplace(id, newTimevector);
	}
	else {
		dpsList[id].push_back(DPS);
		timeList[id].push_back(time);
	}
}

VOID PlotWindow::AddAbData(DOUBLE DPS, DOUBLE time)
{
	if (_abLastTime == time) {
		return;
	}
	_abLastTime = time;

	_abList.push_back(DPS);
	_abTimeList.push_back(time);
}

VOID PlotWindow::AddJqData(BYTE stack, DOUBLE time)
{
	if (_jqLastTime == time) {
		return;
	}
	_jqLastTime = time;

	_jqList.push_back(stack);
	_jqTimeList.push_back(time);
}

VOID PlotWindow::AddAnnonation(string content)
{
	_annonXList.push_back(_abTimeList.back());
	_annonYList.push_back(_abList.back());
	_annonContentList.push_back(content);
}

VOID PlotWindow::OpenWindow()
{
	_isOpen = true;
}

VOID PlotWindow::Update()
{
	if (_isOpen) {

		ImGui::Begin(STR_UTILWINDOW_MEOW, &_isOpen, ImGuiWindowFlags_None);

		if (ImGui::BeginTabBar(u8"테스트2"))
		{
			UpdatePlotTab();
			UpdateAbPlotTab();
			UpdateJqPlotTab();
			UTILLWINDOW.Update();
		}
		ImGui::End();
	}
}

VOID PlotWindow::UpdatePlotTab()
{
	if (ImGui::BeginTabItem(STR_UTILWINDOW_DPSGRAPH))
	{
		if (timeList.size() > 0) {
			UINT32 firstId = metaInfos.front()->_id;
			UINT32 lastId = metaInfos.back()->_id;
			USHORT currentSize = timeList[firstId].size();

			// 가로
			DOUBLE startX = 0.0;
			if (currentSize > 45) {
				startX = timeList[firstId].at(currentSize - 45);
			}
			DOUBLE endX = timeList[firstId].at(currentSize - 1);
			//세로
			DOUBLE startY = 0;
			DOUBLE endY = 10000;
			if (currentSize > 45) {
				auto itr = dpsList[firstId].begin();
				itr += (dpsList[firstId].size() - 1) - (45 - 1);
				for (; itr != dpsList[firstId].end(); itr++) {
					if (*itr > endY) {
						endY = *itr;
					}
				}
			}
			else {
				auto itr = dpsList[firstId].begin();
				for (; itr != dpsList[firstId].end(); itr++) {
					if (*itr > endY) {
						endY = *itr;
					}
				}
			}
			startY = endY - 7000;
			if (startY <= 0) {
				startY = 0;
			}
			endY += 1000;

			if (!_end) {
				ImPlot::SetNextPlotLimitsX(startX, endX, ImGuiCond_Always);
				ImPlot::SetNextPlotLimitsY(startY, endY, ImGuiCond_Always);
			}
		}

		if (ImPlot::BeginPlot(STR_UTILWINDOW_DPSGRAPH, STR_UTILWINDOW_DPSGRAPH_TIME_SEC, "dps", ImVec2(-1, 0), ImPlotFlags_AntiAliased, ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit)) {
			auto it = metaInfos.begin();
			for (; it != metaInfos.end(); it++) {
				UINT32 id = (*it)->_id;
				string name = (*it)->_name;

				ImPlot::PlotLine(name.c_str(), timeList[id].data(), dpsList[id].data(), dpsList[id].size());
			}

			ImPlot::EndPlot();
		}
		ImGui::EndTabItem();
	}
}

VOID PlotWindow::UpdateAbPlotTab()
{
	if (ImGui::BeginTabItem(STR_UTILWINDOW_ABGRAPH))
	{
		UINT32 currentSize = _abTimeList.size();

		DOUBLE startX = 0.0;
		DOUBLE endX = 5.0;
		if (currentSize > 45) {
			startX = _abTimeList.at(currentSize - 45);
		}
		if (currentSize > 0) {
			endX = _abTimeList.back();
		}

		if (!_end) {
			ImPlot::SetNextPlotLimitsX(startX, endX, ImGuiCond_Always);
		}
		ImPlot::SetNextPlotLimitsY(0.0, 100.0, ImGuiCond_Always);
		if (ImPlot::BeginPlot(STR_UTILWINDOW_ABGRAPH, STR_UTILWINDOW_ABGRAPH_TIME_SEC, "ab", ImVec2(-1, 0), ImPlotFlags_AntiAliased, ImPlotAxisFlags_None, ImPlotAxisFlags_None)) {
			ImPlot::PlotLine(STR_TABLE_YOU, _abTimeList.data(), _abList.data(), _abList.size());
			auto itr = _annonXList.begin();
			for (; itr != _annonXList.end(); itr++) {
				int currentIndex = itr - _annonXList.begin();
				ImPlot::Annotate(_annonXList.at(currentIndex),_annonYList.at(currentIndex), ImVec2(15, 15), ImVec4(0.30f, 0.30f, 0.30f, 0.84f), _annonContentList.at(currentIndex).c_str());
			}
			ImPlot::EndPlot();
		}
		ImGui::EndTabItem();
	}
}

VOID PlotWindow::UpdateJqPlotTab()
{
	if (ImGui::BeginTabItem(STR_UTILWINDOW_JQGRAPH))
	{
		UINT32 currentSize = _jqTimeList.size();
		DOUBLE startX = 0.0;
		DOUBLE endX = 5.0;
		if (currentSize > 45) {
			startX = _jqTimeList.at(currentSize - 45);
		}
		if (currentSize > 0) {
			endX = _jqTimeList.back();
		}
		if (!_end) {
			ImPlot::SetNextPlotLimitsX(startX, endX, ImGuiCond_Always);
		}
		ImPlot::SetNextPlotLimitsY(0, 4, ImGuiCond_Always);
		if (ImPlot::BeginPlot(STR_UTILWINDOW_JQGRAPH, STR_UTILWINDOW_JQGRAOH_TIME_SEC, "stack", ImVec2(-1, 0), ImPlotFlags_AntiAliased, ImPlotAxisFlags_None, ImPlotAxisFlags_None)) {
			ImPlot::PlotLine(STR_TABLE_YOU, _jqTimeList.data(), _jqList.data(), _jqList.size());
			ImPlot::EndPlot();
		}
		ImGui::EndTabItem();
	}
}



VOID PlotWindow::End()
{
	_end = true;
}

VOID PlotWindow::Clear()
{
	_end = false;
	metaInfos.clear();
	dpsList.clear();
	timeList.clear();

	_abList.clear();
	_abTimeList.clear();

	_jqList.clear();
	_jqTimeList.clear();

	_lastTime = -1.0;
	_abLastTime = -1.0;
	_jqLastTime = -1.0;
	// TODO : new로 생성한건 delete인가 해야됨
}

PlotWindow::PlotWindow()
{
}

PlotWindow::~PlotWindow()
{
}
