#include "pch.h"
#include ".\Damage Meter\History.h"
#include ".\Damage Meter\Damage Meter.h"

VOID _HISTORYINFO::Setup(HISTORY_DATA* historyData, UINT32 worldID, ULONG64 time) {
	_historyData = historyData;
	_worldID = worldID;
	_time = time;
	GetLocalTime(&_saveTime);
}

VOID _HISTORYINFO::Clear(){
	
	if (_historyData == nullptr)
		return;

	for (auto itr = _historyData->_playerHistory.begin(); itr != _historyData->_playerHistory.end(); itr++)
		delete (*itr);

	for (auto itr = _historyData->_ownerHistory.begin(); itr != _historyData->_ownerHistory.end(); itr++)
		delete (*itr);

	for (auto itr = _historyData->_dbHistory.begin(); itr != _historyData->_dbHistory.end(); itr++)
		delete (*itr);

	for (auto itr = _historyData->_buffHistory.begin(); itr != _historyData->_buffHistory.end(); itr++)
		delete (*itr);

	_historyData->_playerHistory.clear();
	_historyData->_ownerHistory.clear();
	_historyData->_dbHistory.clear();
	_historyData->_buffHistory.clear();
	delete _historyData->_plotHistory;

	delete _historyData;
	_historyData = nullptr;

	_worldID = 0;
	ZeroMemory(&_saveTime, sizeof(SYSTEMTIME));

}

SWDamageMeterHistory::~SWDamageMeterHistory() {
	
	_stopAddHistory = true;

	_mutex.lock();

	for (auto itr = _historys.begin(); itr != _historys.end(); itr++) {
		HISTORY_INFO* hi = (HISTORY_INFO*)*itr;
		hi->Clear();

		delete* itr;
	}

	_historys.clear();

	_mutex.unlock();
}

VOID SWDamageMeterHistory::ClearHistory(INT index) {

	if (index > 0) {
		auto ptr = _historys.begin();

		HISTORY_INFO* hi = (HISTORY_INFO*)*ptr;
		hi->Clear();

		delete* ptr;
		_historys.erase(ptr);
	}
}

VOID SWDamageMeterHistory::push_back(HISTORY_DATA* historyData) {

	if (_stopAddHistory) {
		delete historyData;
		return;
	}

	_mutex.lock();

	ClearHistory(_curIndex / HISTORY_SIZE);

	HISTORY_INFO* hi = new HISTORY_INFO;
	hi->Setup(historyData, DAMAGEMETER.GetWorldID(), DAMAGEMETER.GetTime());

	_historys.push_back(hi);

	_curIndex++;

	_mutex.unlock();
}