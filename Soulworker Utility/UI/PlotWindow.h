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

	vector<metaInfo*> _metaInfos;
	unordered_map<UINT32, vector<double>> _dpsList;
	unordered_map<UINT32, vector<double>> _timeList;
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

	unordered_map<UINT32, vector<double>> _bossHpList;
	unordered_map<UINT32, vector<double>> _bossTimeList;

public:

	VOID AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement);
	VOID AddAbData(DOUBLE DPS, DOUBLE time);
	VOID AddJqData(BYTE stack, DOUBLE time);
	VOID AddAnnonation(string content);
	VOID AddBossHpData(UINT32 id, UINT64 HP, DOUBLE time);
	vector<metaInfo*> GetMetaInfo()
	{
		return _metaInfos;
	}
	unordered_map<UINT32, vector<double>> GetTimeList()
	{
		return _timeList;
	}
	unordered_map<UINT32, vector<double>> GetDPSList()
	{
		return _dpsList;
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
	unordered_map<UINT32, vector<double>> GetBossHpList()
	{
		return _bossHpList;
	}
	unordered_map<UINT32, vector<double>> GetBossTimeList()
	{
		return _bossTimeList;
	}
};

class PlotWindow : public Singleton<PlotWindow> {
private:
	bool _isOpen = false;
	bool _end = false;

	PlotInfo* _pi = nullptr;

	UINT32 _selectedBossHpComboID;

	VOID UpdatePlotTab();
	VOID UpdateAbPlotTab();
	VOID UpdateJqPlotTab();
	VOID UpdateBossHpPlotTab();
	VOID UpdateBossHpPlotCombo();
	VOID UpdateBossHpPlotGraph();
public:

	VOID AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement);
	VOID AddAbData(DOUBLE DPS, DOUBLE time);
	VOID AddJqData(BYTE stack, DOUBLE time);
	VOID AddAnnonation(string content);
	VOID AddBossHpData(UINT32 id, UINT64 HP, DOUBLE time);

	VOID OpenWindow();
	VOID Update();
	VOID End();
	VOID Clear();

	VOID SetPlotInfo(PlotInfo* p_pi);
	PlotInfo* GetPlotInfo();

	PlotWindow() : _selectedBossHpComboID(-1) {}
	~PlotWindow();

};