#include "pch.h"
#include ".\Damage Meter\History.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Damage Meter\SaveData.h"
#include ".\UI\UiWindow.h"

VOID _HISTORYINFO::Setup(HISTORY_DATA* historyData, UINT32 worldID, ULONG64 time, UINT32 myID, BOOL isSaveData, SYSTEMTIME* saveTime, UINT32 realClearTime) {
	_historyData = historyData;
	_worldID = worldID;
	_time = time;
	_myID = myID;
	_isSaveData = isSaveData;
	if (saveTime == nullptr) {
		_saveTime = new SYSTEMTIME;
		GetLocalTime(_saveTime);
	}
	else {
		_saveTime = saveTime;
	}
	_realClearTime = realClearTime;

	if (!isSaveData && UIOPTION.isUseSaveData())
	{
		flatbuffers::FlatBufferBuilder fbb(1024);
		fbb.Finish(Serialization(fbb, historyData));

		SAVEDATA.Save(fbb);
	}
}

VOID _HISTORYINFO::Clear(){
	
	if (_historyData == nullptr)
		return;

	for (auto itr = _historyData->_playerHistory.begin(); itr != _historyData->_playerHistory.end(); itr++)
		delete (*itr);

	for (auto itr = _historyData->_dbHistory.begin(); itr != _historyData->_dbHistory.end(); itr++)
		delete (*itr);

	for (auto itr = _historyData->_buffHistory.begin(); itr != _historyData->_buffHistory.end(); itr++)
		delete (*itr);

	for (auto itr = _historyData->_playerMetadata.begin(); itr != _historyData->_playerMetadata.end(); itr++)
		delete itr->second;

	_historyData->_playerHistory.clear();
	_historyData->_dbHistory.clear();
	_historyData->_buffHistory.clear();
	_historyData->_playerMetadata.clear();

	if (_historyData->_plotHistory != nullptr)
	{
		_historyData->_plotHistory->Clear();
		delete _historyData->_plotHistory;
	}

	if (_historyData->_combatIF != nullptr)
	{
		_historyData->_combatIF->Clear();
		delete _historyData->_combatIF;
	}

	delete _historyData;
	_historyData = nullptr;

	_worldID = 0;
	delete _saveTime;

}

SWDamageMeterHistory::~SWDamageMeterHistory() {
	
	_stopAddHistory = true;

	BOOL a = _mutex.try_lock();

	ClearVector();

	_mutex.unlock();
}

VOID SWDamageMeterHistory::ClearAll()
{
	SAVEDATA.Delete(-1, HISTORY_SIZE);
	ClearVector();
}

VOID SWDamageMeterHistory::ClearVector()
{
	for (auto itr = _historys.begin(); itr != _historys.end(); itr++) {
		HISTORY_INFO* hi = (HISTORY_INFO*)*itr;
		hi->Clear();

		delete (*itr);
	}
	_historys.clear();
	_curIndex = 0;
}

VOID SWDamageMeterHistory::ClearHistory(HISTORY_INFO* pHI, BOOL deleteFirst)
{
	if (_historys.begin() == _historys.end())
		return;

	if (pHI == nullptr)
	{
		pHI = (HISTORY_INFO*)*_historys.begin();
	}

	auto itr = find(_historys.begin(), _historys.end(), (LPVOID)pHI);

	if (UIOPTION.isUseSaveData())
	{
		LONG64 index = 0;
		if (deleteFirst)
			index = 1;
		else {
			if (itr != _historys.end())
				index = (itr - _historys.begin()) + 1;
		}

		SAVEDATA.Delete(index);
	}

	pHI->Clear();

	if (itr != _historys.end())
	{
		_historys.erase(itr);
	}

	delete pHI;
}

VOID SWDamageMeterHistory::push_back(HISTORY_INFO* pHi) {

	BOOL canLock = _mutex.try_lock();
	{
		do
		{
			if (_stopAddHistory) {
				break;
			}

			if (_curIndex >= HISTORY_SIZE)
				ClearHistory();

			_historys.push_back(pHi);

			_curIndex++;
		} while (false);

		if (canLock)
			_mutex.unlock();
	}
}

flatbuffers::Offset<_tHistory> _HISTORYINFO::Serialization(flatbuffers::FlatBufferBuilder& fbb, HISTORY_DATA* historyData)
{
	// _tDamagePlayer
	std::vector<flatbuffers::Offset<_tDamagePlayer>> vDamagePlayer;
	for (auto itr = historyData->_playerHistory.begin(); itr != historyData->_playerHistory.end(); itr++)
		(*itr)->Serialization(fbb, vDamagePlayer);
	auto fcvDamagePlayerList = fbb.CreateVector(vDamagePlayer);

	// _tDB2_Struct
	std::vector<flatbuffers::Offset<_tDB2_Struct>> vDB2Struct;
	for (auto itr = historyData->_dbHistory.begin(); itr != historyData->_dbHistory.end(); itr++)
	{
		_tDB2_StructBuilder tdb2sb(fbb);
		tdb2sb.add__id((*itr)->_id);
		tdb2sb.add__db2((*itr)->_db2);
		tdb2sb.add__type((*itr)->_type);

		vDB2Struct.push_back(tdb2sb.Finish());
	}
	auto fcvDB2Struct = fbb.CreateVector(vDB2Struct);

	// _tBuff
	std::vector<flatbuffers::Offset<_tBuff>> vBuff;
	for (auto itr = historyData->_buffHistory.begin(); itr != historyData->_buffHistory.end(); itr++)
		(*itr)->Serialization(fbb, vBuff);
	auto fcvBuff = fbb.CreateVector(vBuff);

	// _tPlotInfo
	auto fcvPlotInfo = historyData->_plotHistory->Serialization(fbb);

	// _tPlayerMetaData
	std::vector<flatbuffers::Offset<_tPlayerMetaData>> vPlayerMetaData;
	for (auto itr = historyData->_playerMetadata.begin(); itr != historyData->_playerMetadata.end(); itr++)
		itr->second->Serialization(fbb, vPlayerMetaData);
	auto fcvPlayerMetaData = fbb.CreateVector(vPlayerMetaData);

	// _tCombatInterface
	auto fcvCombatInterface = historyData->_combatIF->Serialization(fbb);

	// _tHistory
	auto sTime = _sTime(_saveTime->wYear, _saveTime->wMonth, _saveTime->wDayOfWeek, _saveTime->wDay, _saveTime->wHour, _saveTime->wMinute, _saveTime->wSecond, _saveTime->wMilliseconds);
	auto fcsExtInfo = fbb.CreateString(historyData->_extInfo);

	_tHistoryBuilder thb(fbb);
	
	thb.add__word_id(_worldID);
	thb.add__save_time(&sTime);
	thb.add__time(_time);
	thb.add__my_id(_myID);
	thb.add__ext_info(fcsExtInfo);
	
	thb.add__damage_player(fcvDamagePlayerList);
	thb.add__db2_struct(fcvDB2Struct);
	thb.add__player_buff(fcvBuff);
	thb.add__plot_info(fcvPlotInfo);
	thb.add__player_meta(fcvPlayerMetaData);

	thb.add__real_clear_time(_realClearTime);

	thb.add__combat(fcvCombatInterface);

	return thb.Finish();
}

VOID SWDamageMeterHistory::UnSerialization(const _tHistory* pHistory)
{
	if (_stopAddHistory) {
		return;
	}

	HISTORY_DATA* pHD = new HISTORY_DATA;

	// _tDamagePlayer
	for (auto itr = pHistory->_damage_player()->begin(); itr != pHistory->_damage_player()->end(); itr++)
	{
		SWDamagePlayer* newDamagePlayer = new SWDamagePlayer;
		newDamagePlayer->UnSerialization(*itr);

		pHD->_playerHistory.push_back(newDamagePlayer);
	}

	// _tDB2_Struct
	for (auto itr = pHistory->_db2_struct()->begin(); itr != pHistory->_db2_struct()->end(); itr++)
	{
		SW_DB2_STRUCT* newDB2Struct = new SW_DB2_STRUCT;
		newDB2Struct->_id = itr->_id();
		newDB2Struct->_db2 = itr->_db2();
		newDB2Struct->_type = itr->_type();

		pHD->_dbHistory.push_back(newDB2Struct);
	}

	// _tBuff
	for (auto itr = pHistory->_player_buff()->begin(); itr != pHistory->_player_buff()->end(); itr++)
	{
		PLAYERBUF* newPlayerBuff = new PLAYERBUF;
		newPlayerBuff->UnSerialization(*itr);

		pHD->_buffHistory.push_back(newPlayerBuff);
	}

	// _tPlotInfo
	PlotInfo* newPlotInfo = new PlotInfo;
	newPlotInfo->UnSerialization(pHistory->_plot_info());
	pHD->_plotHistory = newPlotInfo;

	// _tPlayerMetaData
	for (auto itr = pHistory->_player_meta()->begin(); itr != pHistory->_player_meta()->end(); itr++)
	{
		SWDamageMeter::SW_PLAYER_METADATA* pNewPlayerMeta = new SWDamageMeter::SW_PLAYER_METADATA;
		pNewPlayerMeta->UnSerialization(*itr);

		pHD->_playerMetadata[(*itr)->_id()] = pNewPlayerMeta;
	}

	// _tCombatInterface
	if (pHistory->_combat() != nullptr)
	{
		CombatInterface* pCombatInterface = new CombatInterface;
		pCombatInterface->UnSerialization(pHistory->_combat());
		pHD->_combatIF = pCombatInterface;
	}

	// _tHistory
	if (pHistory->_ext_info() != nullptr)
		pHD->_extInfo = pHistory->_ext_info()->c_str();

	// _save_time
	SYSTEMTIME* pSysTime = new SYSTEMTIME;
	pSysTime->wYear = pHistory->_save_time()->year();
	pSysTime->wMonth = pHistory->_save_time()->month();
	pSysTime->wDayOfWeek = pHistory->_save_time()->day_of_week();
	pSysTime->wDay = pHistory->_save_time()->day();
	pSysTime->wHour = pHistory->_save_time()->hour();
	pSysTime->wMinute = pHistory->_save_time()->minute();
	pSysTime->wSecond = pHistory->_save_time()->second();
	pSysTime->wMilliseconds = pHistory->_save_time()->milliseconds();

	HISTORY_INFO* pHI = new HISTORY_INFO;
	pHI->Setup(pHD, pHistory->_word_id(), pHistory->_time(), pHistory->_my_id(), TRUE, pSysTime, pHistory->_real_clear_time());

	BOOL canLock = _mutex.try_lock();
	{
		push_back(pHI);
		if (canLock)
			_mutex.unlock();
	}
}