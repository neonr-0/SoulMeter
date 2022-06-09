#pragma once
#include "pch.h"

#define PLOTWINDOW PlotWindow::getInstance()
#include <vector>
#include <unordered_map>

struct metaInfo {
	metaInfo(UINT32 id, string name) {
		_id = id;
		_name = name;
	}
	UINT32 _id;
	string _name;
};

struct damageInfo {
	damageInfo(double dps, double time) {
		_dps = dps;
		_time = time;
	}
	double _dps;
	double _time;
};

class PlotInfo : public MemoryPool<PlotInfo> {
private:
	UINT32 _id;
	string _name;

	vector<metaInfo*> metaInfos;
	unordered_map<UINT32, vector<double>> dpsList;
	unordered_map<UINT32, vector<double>> timeList;
	double _lastTime = -1;
	bool _allowed = false;

	vector<double> _abList;
	vector<double> _abTimeList;
	double _abLastTime = -1;

	vector<double> _jqList;
	vector<double> _jqTimeList;
	double _jqLastTime = -1;

	vector<double> _annonXList;
	vector<double> _annonYList;
	vector<string> _annonContentList;

protected:
	PlotInfo() {}
	PlotInfo(const PlotInfo& other) {}

public:
	PlotInfo::PlotInfo(UINT32 id, string name);

	VOID AddData(DOUBLE DPS, DOUBLE time, bool isFirstElement);
	VOID AddAbData(DOUBLE DPS, DOUBLE time);
	VOID AddJqData(BYTE stack, DOUBLE time);
	VOID AddAnnonation(string content);
	vector<metaInfo*> GetMetaInfo()
	{
		return metaInfos;
	}
	unordered_map<UINT32, vector<double>> GetTimeList()
	{
		return timeList;
	}
	unordered_map<UINT32, vector<double>> GetDPSList()
	{
		return dpsList;
	}
	vector<double> GetABList()
	{
		return _abList;
	}
	vector<double> GetABTimeList()
	{
		return _abTimeList;
	}
	vector<double> GetJQList()
	{
		return _jqList;
	}
	vector<double> GetJQTimeList()
	{
		return _jqTimeList;
	}
	vector<double> GetAnnonXList()
	{
		return _annonXList;
	}
	vector<double> GetAnnonYList()
	{
		return _annonYList;
	}
	vector<string> GetAnnonContentList()
	{
		return _annonContentList;
	}
};

class PlotWindow : public Singleton<PlotWindow> {
private:
	bool _isOpen = false;
	bool _end = false;

	PlotInfo* pi = nullptr;

	VOID UpdatePlotTab();
	VOID UpdateAbPlotTab();
	VOID UpdateJqPlotTab();
public:
	VOID AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement);
	VOID AddAbData(DOUBLE DPS, DOUBLE time);
	VOID AddJqData(BYTE stack, DOUBLE time);
	VOID AddAnnonation(string content);

	VOID OpenWindow();
	VOID Update();
	VOID End();
	VOID Clear();

	PlotInfo* PlotWindow::GetPlotInfo();
	VOID PlotWindow::SetPlotInfo(PlotInfo* p_pi);

	PlotWindow();
	~PlotWindow();

};