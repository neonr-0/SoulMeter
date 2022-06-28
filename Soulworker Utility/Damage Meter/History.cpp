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
	for (INT i = 0; i < HISTORY_SIZE; i++) {
		_historys[i].Clear();
	}
}

VOID SWDamageMeterHistory::ClearHistory(INT index) {

	_historys[index].Clear();
}

VOID SWDamageMeterHistory::push_back(HISTORY_DATA* historyData) {

	ClearHistory(_curIndex % HISTORY_SIZE);
	_historys[_curIndex++ % HISTORY_SIZE].Setup(historyData, DAMAGEMETER.GetWorldID(), DAMAGEMETER.GetTime());
}

const HISTORY_INFO& SWDamageMeterHistory::operator[](INT index) {
	
	if (index >= HISTORY_SIZE || index < 0)
		assert(FALSE);

	return _historys[index];
}

SIZE_T SWDamageMeterHistory::size() {

	if (_curIndex > HISTORY_SIZE)
		return HISTORY_SIZE;
	else
		return _curIndex % HISTORY_SIZE;
}