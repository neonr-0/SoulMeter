#include "pch.h"
#include "PlotWindow.h"
#include "UtillWindow.h"
#include <vector>
#include ".\Language\Region.h"

PlotInfo::PlotInfo(UINT32 id, string name) : _id(id), _name(name) {}

VOID PlotWindow::AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement)
{
	if (pi == nullptr)
		pi = new PlotInfo(id, name);

	pi->AddData(DPS, time, isFirstElement);
}

VOID PlotInfo::AddData(DOUBLE DPS, DOUBLE time, bool isFirstElement)
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
	// 
	else {
		if (!_allowed) {
			return;
		}
	}
	_lastTime = time;

	auto it = metaInfos.begin();
	bool metaInfoFound = false;
	for (; it != metaInfos.end(); it++) {
		if ((*it)->_id == _id) {
			metaInfoFound = true;
			break;
		}
	}

	if (!metaInfoFound) {
		metaInfos.push_back(new metaInfo(_id, _name));

		vector<double> newDPSvector;
		newDPSvector.push_back(DPS);
		dpsList.emplace(_id, newDPSvector);

		vector<double> newTimevector;
		newTimevector.push_back(time);
		timeList.emplace(_id, newTimevector);
	}
	else {
		dpsList[_id].push_back(DPS);
		timeList[_id].push_back(time);
	}
}

VOID PlotWindow::AddAbData(DOUBLE DPS, DOUBLE time)
{
	if (pi == nullptr)
		return;
	pi->AddAbData(DPS, time);
}

VOID PlotInfo::AddAbData(DOUBLE DPS, DOUBLE time)
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
	if (pi == nullptr)
		return;
	pi->AddJqData(stack, time);
}

VOID PlotInfo::AddJqData(BYTE stack, DOUBLE time)
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
	if (pi == nullptr)
		return;
	pi->AddAnnonation(content);
}

VOID PlotInfo::AddAnnonation(string content)
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
			if (pi != nullptr) {
				UpdatePlotTab();
				UpdateAbPlotTab();
				UpdateJqPlotTab();
			}
			UTILLWINDOW.Update();
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

VOID PlotWindow::UpdatePlotTab()
{
	if (ImGui::BeginTabItem(STR_UTILWINDOW_DPSGRAPH))
	{
		auto timeList = pi->GetTimeList();
		auto dpsList = pi->GetDPSList();
		auto metaInfos = pi->GetMetaInfo();
		if (timeList.size() > 0) {
			UINT32 firstId = metaInfos.front()->_id;
			UINT32 lastId = metaInfos.back()->_id;
			USHORT currentSize = timeList[firstId].size();

			// 
			DOUBLE startX = 0.0;
			if (currentSize > 45) {
				startX = timeList[firstId].at(currentSize - 45);
			}
			DOUBLE endX = timeList[firstId].at(currentSize - 1);
			//
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

		if (ImPlot::BeginPlot(STR_UTILWINDOW_DPSGRAPH, STR_UTILWINDOW_DPSGRAPH_TIME_SEC, STR_UTILWINDOW_DPSGRAPH, ImVec2(-1, 0), ImPlotFlags_AntiAliased, ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit)) {
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
		auto _abTimeList = pi->GetABTimeList();
		auto _abList = pi->GetABList();
		auto _annonXList = pi->GetAnnonXList();
		auto _annonYList = pi->GetAnnonYList();
		auto _annonContentList = pi->GetAnnonContentList();
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
		if (ImPlot::BeginPlot(STR_UTILWINDOW_ABGRAPH, STR_UTILWINDOW_ABGRAPH_TIME_SEC, STR_UTILWINDOW_ABGRAPH, ImVec2(-1, 0), ImPlotFlags_AntiAliased, ImPlotAxisFlags_None, ImPlotAxisFlags_None)) {
			ImPlot::PlotLine(STR_TABLE_YOU, _abTimeList.data(), _abList.data(), _abList.size());
			auto itr = _annonXList.begin();
			for (; itr != _annonXList.end(); itr++) {
				int currentIndex = itr - _annonXList.begin();
				ImPlot::Annotate(_annonXList.at(currentIndex), _annonYList.at(currentIndex), ImVec2(15, 15), ImVec4(0.30f, 0.30f, 0.30f, 0.84f), _annonContentList.at(currentIndex).c_str());
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
		auto _jqTimeList = pi->GetJQTimeList();
		auto _jqList = pi->GetJQList();
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
		if (ImPlot::BeginPlot(STR_UTILWINDOW_JQGRAPH, STR_UTILWINDOW_JQGRAOH_TIME_SEC, STR_UTILWINDOW_JQGRAPH, ImVec2(-1, 0), ImPlotFlags_AntiAliased, ImPlotAxisFlags_None, ImPlotAxisFlags_None)) {
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
	pi = nullptr;
}

VOID PlotWindow::SetPlotInfo(PlotInfo* p_pi)
{
	pi = p_pi;
	_end = true;
}

PlotInfo* PlotWindow::GetPlotInfo()
{
	return pi;
}

PlotWindow::PlotWindow()
{
}

PlotWindow::~PlotWindow()
{
}
