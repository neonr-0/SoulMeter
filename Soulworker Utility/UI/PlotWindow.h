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

class PlotWindow : public Singleton<PlotWindow> {
private:
	vector<metaInfo*> metaInfos;
	unordered_map<UINT32, vector<double>> dpsList;
	unordered_map<UINT32, vector<double>> timeList;
	bool _isOpen = false;
	bool _allowed = false;
	double _lastTime = -1;

	vector<double> _abList;
	vector<double> _abTimeList;
	double _abLastTime = -1;

	vector<double> _jqList;
	vector<double> _jqTimeList;
	double _jqLastTime = -1;

	vector<double> _annonXList;
	vector<double> _annonYList;
	vector<string> _annonContentList;

	bool _end = false;

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

	PlotWindow();
	~PlotWindow();

};