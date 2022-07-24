#pragma once
#include "pch.h"

#define PLOTWINDOW PlotWindow::getInstance()
#include <vector>
#include <unordered_map>
#include ".\FlatBuffers\include\SW_PLOTINFO_.h"
#include ".\Damage Meter\FlatBufferUtil.h"

using namespace std;
using namespace SoulMeterFBS::History;

struct metaInfo {
	metaInfo(UINT32 id, string name) {
		_id = id;
		_name = name;
	}
	UINT32 _id;
	string _name;
};

class PlotInfo : public MemoryPool<PlotInfo> {
private:

	vector<metaInfo*> _metaInfos;
	unordered_map<UINT32, vector<double>> _dpsList;
	unordered_map<UINT32, vector<double>> _timeList;
	double _lastTime = -1;

	vector<double> _abList;
	vector<double> _abTimeList;
	double _abLastTime = -1;

	vector<double> _bdList;
	vector<double> _bdTimeList;
	double _bdLastTime = -1;

	vector<double> _jqList;
	vector<double> _jqTimeList;
	double _jqLastTime = -1;

	unordered_map<UINT32, vector<double>> _bossHpList;
	unordered_map<UINT32, vector<double>> _bossTimeList;
	double _bhLastTime = -1;

	bool _allowed = false;

	BOOL _isHistoryMode = FALSE;

public:

	VOID AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement);
	VOID AddAbData(DOUBLE DPS, DOUBLE time);
	VOID AddBdData(DOUBLE DPS, DOUBLE time);
	VOID AddJqData(BYTE stack, DOUBLE time);
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
	vector<double> GetBDList()
	{
		return _bdList;
	}
	vector<double> GetBDTimeList()
	{
		return _bdTimeList;
	}
	vector<double> GetJQList()
	{
		return _jqList;
	}
	vector<double> GetJQTimeList()
	{
		return _jqTimeList;
	}
	unordered_map<UINT32, vector<double>> GetBossHpList()
	{
		return _bossHpList;
	}
	unordered_map<UINT32, vector<double>> GetBossTimeList()
	{
		return _bossTimeList;
	}

	VOID Clear()
	{
		for (auto itr = _metaInfos.begin(); itr != _metaInfos.end(); itr++)
			delete (*itr);
	}
	flatbuffers::Offset<_tPlotInfo> Serialization(flatbuffers::FlatBufferBuilder& fbb)
	{
		// _tMetaInfo
		std::vector<flatbuffers::Offset<_tMetaInfo>> vMetaInfo;
		for (auto itr = _metaInfos.begin(); itr != _metaInfos.end(); itr++)
		{
			auto fcsName = fbb.CreateString((*itr)->_name);

			_tMetaInfoBuilder tmib(fbb);

			tmib.add__id((*itr)->_id);
			tmib.add__name(fcsName);

			vMetaInfo.push_back(tmib.Finish());
		}
		auto fcvMetaInfo = fbb.CreateVector(vMetaInfo);

		// _tIntDoubleList
		std::vector<flatbuffers::Offset<_tIntDoubleList>> vIntDouble;

		// DPS
		FLATBUFFERUTIL.tIntDoubleListConverter(fbb, vIntDouble, _dpsList);
		auto fcvDpsList = fbb.CreateVector(vIntDouble);

		vIntDouble.clear();
		FLATBUFFERUTIL.tIntDoubleListConverter(fbb, vIntDouble, _timeList);
		auto fcvDpsTimeList = fbb.CreateVector(vIntDouble);

		// BossHP
		vIntDouble.clear();
		FLATBUFFERUTIL.tIntDoubleListConverter(fbb, vIntDouble, _bossHpList);
		auto fcvBossHpList = fbb.CreateVector(vIntDouble);

		vIntDouble.clear();
		FLATBUFFERUTIL.tIntDoubleListConverter(fbb, vIntDouble, _bossTimeList);
		auto fcvBossTimeList = fbb.CreateVector(vIntDouble);

		// vector<double>
		auto fcvABList = fbb.CreateVector(_abList);
		auto fcvABTimeList = fbb.CreateVector(_abTimeList);

		auto fcvBDList = fbb.CreateVector(_bdList);
		auto fcvBDTimeList = fbb.CreateVector(_bdTimeList);

		auto fcvJQList = fbb.CreateVector(_jqList);
		auto fcvJQTimeList = fbb.CreateVector(_jqTimeList);

		// _tPlotInfo
		_tPlotInfoBuilder tpib(fbb);

		tpib.add__meta(fcvMetaInfo);

		tpib.add__dps_list(fcvDpsList);
		tpib.add__dps_time_list(fcvDpsTimeList);

		tpib.add__ab_list(fcvABList);
		tpib.add__ab_time_list(fcvABTimeList);

		tpib.add__bd_list(fcvBDList);
		tpib.add__bd_time_list(fcvBDTimeList);

		tpib.add__jq_list(fcvJQList);
		tpib.add__jq_time_list(fcvJQTimeList);

		tpib.add__boss_hp_list(fcvBossHpList);
		tpib.add__boss_time_list(fcvBossTimeList);

		return tpib.Finish();
	}
	VOID UnSerialization(const _tPlotInfo* tPlotInfo)
	{
		// _tMetaInfo
		for (auto itr = tPlotInfo->_meta()->begin(); itr != tPlotInfo->_meta()->end(); itr++)
		{
			metaInfo* newMetaInfo = new metaInfo(itr->_id(), itr->_name()->c_str());
			_metaInfos.push_back(newMetaInfo);
		}

		// _tIntDoubleList
		// DPS
		for (auto itr = tPlotInfo->_dps_list()->begin(); itr != tPlotInfo->_dps_list()->end(); itr++)
		{
			FLATBUFFERUTIL.tIntDoubleListReverser(*itr, _dpsList);
		}
		for (auto itr = tPlotInfo->_dps_time_list()->begin(); itr != tPlotInfo->_dps_time_list()->end(); itr++)
		{
			FLATBUFFERUTIL.tIntDoubleListReverser(*itr, _timeList);
		}

		// BOSS HP
		for (auto itr = tPlotInfo->_boss_hp_list()->begin(); itr != tPlotInfo->_boss_hp_list()->end(); itr++)
		{
			FLATBUFFERUTIL.tIntDoubleListReverser(*itr, _bossHpList);
		}
		for (auto itr = tPlotInfo->_boss_time_list()->begin(); itr != tPlotInfo->_boss_time_list()->end(); itr++)
		{
			FLATBUFFERUTIL.tIntDoubleListReverser(*itr, _bossTimeList);
		}

		// vector<double>
		FLATBUFFERUTIL.fbvDoubleReverser(tPlotInfo->_ab_list(), _abList);
		FLATBUFFERUTIL.fbvDoubleReverser(tPlotInfo->_ab_time_list(), _abTimeList);

		FLATBUFFERUTIL.fbvDoubleReverser(tPlotInfo->_bd_list(), _bdList);
		FLATBUFFERUTIL.fbvDoubleReverser(tPlotInfo->_bd_time_list(), _bdTimeList);

		FLATBUFFERUTIL.fbvDoubleReverser(tPlotInfo->_jq_list(), _jqList);
		FLATBUFFERUTIL.fbvDoubleReverser(tPlotInfo->_jq_time_list(), _jqTimeList);

		_isHistoryMode = TRUE;
	}
};

class PlotWindow : public Singleton<PlotWindow> {
private:
	bool _isOpen = false;
	bool _end = false;

	PlotInfo* _pi = nullptr;

	UINT32 _selectedBossHpComboID;

	BOOL _historyMode = false;

	VOID UpdatePlotTab();
	VOID UpdateAbPlotTab();
	VOID UpdateBdPlotTab();
	VOID UpdateJqPlotTab();
	VOID UpdateBossHpPlotTab();
	VOID UpdateBossHpPlotCombo();
	VOID UpdateBossHpPlotGraph();
public:

	VOID AddData(UINT32 id, string name, DOUBLE DPS, DOUBLE time, bool isFirstElement);
	VOID AddAbData(DOUBLE DPS, DOUBLE time);
	VOID AddBdData(DOUBLE DPS, DOUBLE time);
	VOID AddJqData(BYTE stack, DOUBLE time);
	VOID AddBossHpData(UINT32 id, UINT64 HP, DOUBLE time);

	VOID OpenWindow();
	VOID Update();
	VOID Start();
	VOID End();
	VOID Clear();

	VOID SetPlotInfo(PlotInfo* p_pi);
	PlotInfo* GetPlotInfo();

	PlotWindow() : _selectedBossHpComboID(-1) {}
	~PlotWindow();

};