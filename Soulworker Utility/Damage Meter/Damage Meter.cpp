#include "pch.h"
#include ".\Damage Meter\Timer.h"
#include ".\Damage Meter\SWDamagePlayer.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\UI\PlotWindow.h"
#include ".\Damage Meter\History.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\Combat Meter\CombatMeter.h"
#include ".\UI\UtillWindow.h"
#include ".\Damage Meter\MapList.h"

SWDamageMeter::~SWDamageMeter() {

	BOOL canLock = _mutex.try_lock();

	if (_historyMode)
		Restore();

	for (auto itr = _playerInfo.begin(); itr != _playerInfo.end(); itr++)
		delete (*itr);

	for (auto itr = _ownerInfo.begin(); itr != _ownerInfo.end(); itr++)
		delete (*itr);

	ClearDB();

	for (auto itr = _playerMetadata.begin(); itr != _playerMetadata.end(); itr++)
		delete (itr->second);

	_playerInfo.clear();
	_ownerInfo.clear();
	_dbInfo.clear();
	_playerMetadata.clear();

	_historyPlayerInfo.clear();
	_historyDbInfo.clear();
	_historyPlayerMetadata.clear();

	_playerUseAwaken.clear();

	FreeLock();
}

VOID SWDamageMeter::GetLock() {
	_mutex.lock();
}
VOID SWDamageMeter::FreeLock() {
	_mutex.unlock();
}

VOID SWDamageMeter::InsertPlayerInfo(UINT32 id, UINT64 totalDMG, UINT64 soulstoneDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID) {
	auto itr = _playerInfo.begin();

	for (; itr != _playerInfo.end(); itr++) {
		if (id == (*itr)->GetID()) {
			(*itr)->AddDamage(totalDMG, soulstoneDMG, damageType, maxCombo, monsterID, skillID);
			return;
		}
	}

	_playerInfo.push_back(new SWDamagePlayer(id, totalDMG, soulstoneDMG, damageType, maxCombo, monsterID, skillID));
}

VOID SWDamageMeter::AddDamage(UINT32 id, UINT64 totalDMG, UINT64 soulstoneDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID) {
	if (_mazeEnd)
		return;

	USHORT usWorldID = GetWorldID();
	// Meter must know mazeId to work correctly
	if (usWorldID == 0)
		return;

	SW_DB2_STRUCT* db = DAMAGEMETER.GetMonsterDB(monsterID);
	UINT32 monsterId = 0;
	bool bIsListBoss = false, bIsListMap = false;
	if (db != nullptr) {
		monsterId = db->_db2;
	}

	auto stList = StrictModeList.find(usWorldID);
	if (stList != StrictModeList.end())
	{
		auto& vec = (*stList).second;
		auto find = std::find(vec.begin(), vec.end(), monsterId);
		if (find != vec.end())
			bIsListBoss = true;
		bIsListMap = true;
	}

	if (!isRun()) {
		// Do not start/resume if it is damage to Puppet Orb,Ephnel Octagon, or Nabi Bomb wick
		if (resumeIgnoreIdList.find(monsterId) != resumeIgnoreIdList.end()) {
			return;
		}

		// only damage in list Boss can start/resume timer.
		if (bIsListMap && !bIsListBoss)
			return;
	}

	if (totalDMG > 0)
	{
		CombatLog* pCombatLog = new CombatLog;
		pCombatLog->_type = COMBATMETER.ConvertDamageTypeForGiveDamage(damageType);
		pCombatLog->_val1 = static_cast<DOUBLE>(totalDMG);
		pCombatLog->_val2 = static_cast<DOUBLE>(soulstoneDMG);
		COMBATMETER.Insert(id, CombatType::PLAYER, pCombatLog);
	}
	else {
		// ignore if rank map
		if (rankMap.find(usWorldID) == rankMap.end())
		{
			// suspend by boss is god mode
			if ((bIsListMap && bIsListBoss) || (db != NULL && db->_type == 4))
			{
				DAMAGEMETER.Suspend();
				return;
			}
		}
	}

	Start();
	InsertPlayerInfo(id, totalDMG, soulstoneDMG, damageType, maxCombo, monsterID, skillID);
	Sort();
}

VOID SWDamageMeter::AddPlayerGetDamage(UINT32 playerId, UINT64 totalDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, UINT32 monsterID, UINT32 skillID)
{
	if (!isRun() || GetTime() == 0)
		return;

	if (CheckPlayer(playerId))
	{
		UINT32 monsterDB2 = 0;
		SW_DB2_STRUCT* db = DAMAGEMETER.GetMonsterDB(monsterID);
		if (db != nullptr) {
			monsterDB2 = db->_db2;
		}

		CombatLog* pCombatLog = new CombatLog;
		pCombatLog->_type = COMBATMETER.ConvertDamageTypeForTakeDamage(damageType);
		pCombatLog->_val1 = static_cast<DOUBLE>(totalDMG);
		COMBATMETER.Insert(playerId, CombatType::PLAYER, pCombatLog);

		pCombatLog = new CombatLog;
		pCombatLog->_type = COMBATMETER.ConvertDamageTypeForGiveDamage(damageType);
		pCombatLog->_val1 = static_cast<DOUBLE>(totalDMG);
		pCombatLog->_val2 = playerId;
		COMBATMETER.Insert(monsterDB2, CombatType::MONSTER, pCombatLog);
	}

	auto itr = _playerInfo.begin();

	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->AddGetDamage(totalDMG, damageType, monsterID, skillID);
			return;
		}
	}

	if (CheckPlayer(playerId)) {
		SWDamagePlayer* newPlayer = new SWDamagePlayer(playerId);
		newPlayer->AddGetDamage(totalDMG, damageType, monsterID, skillID);
		_playerInfo.push_back(newPlayer);
	}
}

VOID SWDamageMeter::AddEnlighten(UINT32 playerId, FLOAT value)
{
	if (!isRun() || GetTime() == 0)
		return;

	auto itr = _playerInfo.begin();

	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->AddEnlighten(value);
			return;
		}
	}

	if (CheckPlayer(playerId)) {
		SWDamagePlayer* newPlayer = new SWDamagePlayer(playerId);
		newPlayer->AddEnlighten(value);
		_playerInfo.push_back(newPlayer);
	}
}

VOID SWDamageMeter::AddSkillUsed(UINT32 playerId, UINT32 skillId)
{

	if (!isRun() || GetTime() == 0)
		return;

	// counts only skill that can proc enlighten (example : not right click)
	bool canProcEnlighten = false;
	if (skillId >= 10000000 && skillId < 1000000000) {
		if ((skillId % 100000) / 10000 == 0) {
			canProcEnlighten = true;
		}
	}
	if (!canProcEnlighten || !DAMAGEMETER.CheckPlayer(playerId)) {
		return;
	}

	if (std::find(_dwSkills.begin(), _dwSkills.end(), skillId) != _dwSkills.end())
	{
		DAMAGEMETER.AddAwakenPlayer(playerId);
	}

	auto itr = _playerInfo.begin();
	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->AddSkillUsed(skillId);
			return;
		}
	}

	SWDamagePlayer* newPlayer = new SWDamagePlayer(playerId);
	newPlayer->AddSkillUsed(skillId);
	_playerInfo.push_back(newPlayer);
}

VOID SWDamageMeter::AddDodgeUsed(UINT32 playerId)
{
	if (!isRun() || GetTime() == 0)
		return;

	auto itr = _playerInfo.begin();

	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->AddDodgeUsed();
			return;
		}
	}

	SWDamagePlayer* newPlayer = new SWDamagePlayer(playerId);
	newPlayer->AddDodgeUsed();
	_playerInfo.push_back(newPlayer);
}

VOID SWDamageMeter::AddDeath(UINT32 playerId)
{
	if (!isRun() || GetTime() == 0)
		return;

	auto itr = _playerInfo.begin();

	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->AddDeathCount();
			return;
		}
	}

	SWDamagePlayer* newPlayer = new SWDamagePlayer(playerId);
	newPlayer->AddDeathCount();
	_playerInfo.push_back(newPlayer);
}

VOID SWDamageMeter::BuffIn(UINT32 playerId, USHORT buffId, BYTE stack, UINT32 giverId)
{
	if (_mazeEnd)
		return;
	if (buffId != 60228)
		return;

	auto itr = _playerInfo.begin();
	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->SetJqStack(stack);
			return;
		}
	}
}

VOID SWDamageMeter::BuffOut(UINT32 playerId, USHORT buffId)
{
	if (_mazeEnd)
		return;
	// The Real Army 
	if (buffId != 60228)
		return;

	auto itr = _playerInfo.begin();
	for (; itr != _playerInfo.end(); itr++) {
		if (playerId == (*itr)->GetID()) {
			(*itr)->SetJqStack(0);
			return;
		}
	}
}

VOID SWDamageMeter::InsertOwnerID(UINT32 id, UINT32 owner_id) {

	auto itr = _ownerInfo.begin();

	for (; itr != _ownerInfo.end(); itr++) {
		if ((*itr)->_id == id) {
#if DEBUG_DAMAGEMETER_OWNER_ID == 1
			Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [INSERT OWNER] [MODIFY] [ID = %08x] [OWNER = %08x]")), id, owner_id);
#endif
			(*itr)->_owner_id = owner_id;
			return;
		}
	}

	SW_OWNER_ID_STRUCT* obj = new SW_OWNER_ID_STRUCT;
	obj->_id = id;
	obj->_owner_id = owner_id;

#if DEBUG_DAMAGEMETER_OWNER_ID == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [INSERT OWNER] [NEW] [ID = %08x] [OWNER = %08x]")), obj->_id, obj->_owner_id);
#endif

	_ownerInfo.push_back(obj);
}

UINT32 SWDamageMeter::GetOwnerID(UINT32 id) {

	auto itr = _ownerInfo.begin();

	for (; itr != _ownerInfo.end(); itr++) {
		if ((*itr)->_id == id)
			return (*itr)->_owner_id;
	}

	return 0xffffffff;
}

VOID SWDamageMeter::InsertPlayerMetadata(UINT32 id, CHAR* str, BYTE job) {

	auto search = _playerMetadata.find(id);

	if (search == _playerMetadata.end()) {
		SW_PLAYER_METADATA* metaData = new SW_PLAYER_METADATA;
		metaData->_id = id;
		metaData->_job = job;
		strcpy_s(metaData->_name, MAX_NAME_LEN, str);
		_playerMetadata[id] = metaData;

#if DEBUG_DAMAGEMETER_PLAYERMETA == 1
		Log::WriteLogA(const_cast<CHAR*>("[DEBUG] [INSERT PLAYER META] [NEW] [ID = %08x] [NAME = %s] [JOB = %x]"), id, str, job);
#endif
		return;
	}
	SW_PLAYER_METADATA* metaData = search->second;
	metaData->_id = id;
	metaData->_job = job;
	strcpy_s(metaData->_name, MAX_NAME_LEN, str);
}

const CHAR* SWDamageMeter::GetPlayerName(UINT32 id) {
	if ((_historyMode && id == _historyMyID) || (!_historyMode && id == _myID))
		return LANGMANAGER.GetText("STR_TABLE_YOU");

	auto search = GetPlayerMetaDataByHistory()->find(id);
	if (search == GetPlayerMetaDataByHistory()->end()) {
		return LANGMANAGER.GetText("PLAYER_NAME_CANT_FIND");
	}
	return search->second->_name;
}

BYTE SWDamageMeter::GetPlayerJob(UINT32 id) {

	auto search = GetPlayerMetaDataByHistory()->find(id);
	if (search == GetPlayerMetaDataByHistory()->end()) {
		return PLAYER_JOB_CANT_FIND;
	}
	return search->second->_job;
}

SWDamageMeter::SW_PLAYER_METADATA* SWDamageMeter::GetPlayerMetaDataIfNotExistsCreate(UINT32 id)
{
	SW_PLAYER_METADATA* metaData;

	auto search = _playerMetadata.find(id);
	if (search == _playerMetadata.end()) {
		metaData = new SW_PLAYER_METADATA;
		metaData->_id = id;
		strcpy_s(metaData->_name, MAX_NAME_LEN, LANGMANAGER.GetText("PLAYER_NAME_CANT_FIND"));
		metaData->_job = PLAYER_JOB_CANT_FIND;
		_playerMetadata[id] = metaData;
	}
	else {
		metaData = search->second;
	}

	return metaData;
}

VOID SWDamageMeter::UpdateSpecialStat(UINT32 id, USHORT statType, FLOAT statValue)
{
	UpdateStat(id, statType, statValue, TRUE);
}

VOID SWDamageMeter::UpdateStat(UINT32 id, USHORT statType, FLOAT statValue, BOOL isSpecial)
{
	//if (_historyMode) {
	//	return;
	//}
	
	SW_PLAYER_METADATA* metaData = GetPlayerMetaDataIfNotExistsCreate(id);

	if (isSpecial)
		metaData->UpdateSpecialStat(statType, statValue);
	else
		metaData->UpdateStat(statType, statValue);

	return;
}


VOID SWDamageMeter::Sort() {
	sort(_playerInfo.begin(), _playerInfo.end(), SWDamagePlayer::SortFunction);
}

VOID SWDamageMeter::InsertDB(UINT32 id, UINT32 db2) {
	auto itr = _dbInfo.begin();

	for (; itr != _dbInfo.end(); itr++) {
		if ((*itr)->_id == id) {
#if DEBUG_DAMAGEMETER_DB == 1
			Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [INSERT DB] [MODIFY] [ID = %04x] [DB1 = %d] [DB2 = %d]")), id, GetWorldID(), db2);
#endif
			(*itr)->_db2 = db2;
			return;
		}
	}

	INT32 type = -1;
	SWDB.GetMonsterType(db2, &type);

	SW_DB2_STRUCT* db = new SW_DB2_STRUCT;
	db->_id = id;
	db->_db2 = db2;
	db->_type = type;

#if DEBUG_DAMAGEMETER_DB == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [INSERT DB] [NEW] [ID = %04x] [DB2 = %d]")), db->_id, db->_db2);
#endif

	_dbInfo.push_back(db);
}

SW_DB2_STRUCT* SWDamageMeter::GetMonsterDB(UINT32 id) {

	for (auto itr = _dbInfo.begin(); itr != _dbInfo.end(); itr++) {
#if DEBUG_DAMAGEMETER_DB == 1
		Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [GetMonsterDB] [FIND ID = %04x] [ID = %04x] [DB2 = %d]")), id, (*itr)->_id, (*itr)->_db2);
#endif
		if ((*itr)->_id == id) {
			return *itr;
		}
	}

	return nullptr;
}

VOID SWDamageMeter::SetWorldID(USHORT worldID) {
	_worldID = worldID;

#if DEBUG_DAMAGEMETER_WORLD == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [Set World] [World ID = %d]")), _worldID);
#endif
}

USHORT SWDamageMeter::GetWorldID() {

	if (!_historyMode)
		return _worldID;
	else
		return _historyWorldID;
}

BOOL SWDamageMeter::isTownMap() {

	return townMap.find(_worldID) != townMap.end();
}

const CHAR* SWDamageMeter::GetWorldName() {

	if (!_historyMode)
		SWDB.GetMapName(_worldID, _mapName, MAX_MAP_LEN);
	else
		SWDB.GetMapName(_historyWorldID, _mapName, MAX_MAP_LEN);

	return _mapName;
}

VOID SWDamageMeter::SetAggro(UINT32 id, UINT32 targetedId)
{
	if (CheckPlayer(id)) {
		return;
	}

	SW_DB2_STRUCT* db = DAMAGEMETER.GetMonsterDB(id);
	if (db != nullptr) {
		UINT32 db2 = db->_db2;

		// usually type 4 only 1 monster
		if (changeAggroIdList.find(db2) != changeAggroIdList.end() || db->_type == 4) {
			_aggroedId = targetedId;

			SW_PLAYER_METADATA* metaData = GetPlayerMetaData(targetedId);
			for (auto itr = _playerMetadata.begin(); itr != _playerMetadata.end(); itr++)
				itr->second->UpdateAggroTime(metaData == itr->second);
		}
	}
}

UINT32 SWDamageMeter::GetAggro()
{
	return _aggroedId;
}

VOID SWDamageMeter::SetMyID(UINT32 id) {
	_myID = id;

#if DEBUG_DAMAGEMETER_MYID == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("[DEBUG] [Set MyID] [My ID = %08x]")), _myID);
#endif
}

UINT32 SWDamageMeter::GetMyID(BOOL useHistoryID) {
	if (useHistoryID && _historyMode)
		return _historyMyID;
	return _myID;
}

BOOL SWDamageMeter::CheckPlayer(UINT32 id) {
	if (id < 10000000)
		return TRUE;
	else
		return FALSE;
}

vector<SWDamagePlayer*>* SWDamageMeter::GetPlayerInfoByHistory()
{
	if (_historyMode)
		return &_historyPlayerInfo;
	else
		return &_playerInfo;
}

vector<SWDamagePlayer*>::const_iterator SWDamageMeter::GetPlayerInfo(UINT32 id) {

	auto itr = GetPlayerInfoByHistory()->begin();
	for (; itr != GetPlayerInfoByHistory()->end(); itr++) {
		if ((*itr)->GetID() == id) {
			return itr;
		}
	}

	return itr;
}

UINT64 SWDamageMeter::GetPlayerTotalDamage() {

	UINT64 playerTotalDamage = 0;

	for (auto itr = GetPlayerInfoByHistory()->begin(); itr != GetPlayerInfoByHistory()->end(); itr++) {
		playerTotalDamage += (*itr)->GetDamage();
	}

	return playerTotalDamage;
}

vector<SWDamagePlayer*>::const_iterator SWDamageMeter::begin() {
	return GetPlayerInfoByHistory()->begin();
}

vector<SWDamagePlayer*>::const_iterator SWDamageMeter::end() {
	return GetPlayerInfoByHistory()->end();
}

const SIZE_T SWDamageMeter::size() {
	return GetPlayerInfoByHistory()->size();
}

unordered_map<UINT32, SWDamageMeter::SW_PLAYER_METADATA*>* SWDamageMeter::GetPlayerMetaDataByHistory()
{
	if (_historyMode)
		return &_historyPlayerMetadata;
	else
		return &_playerMetadata;
}

SWDamageMeter::SW_PLAYER_METADATA* SWDamageMeter::GetPlayerMetaData(UINT32 id)
{
	auto search = GetPlayerMetaDataByHistory()->find(id);
	if (search == GetPlayerMetaDataByHistory()->end()) {
		return nullptr;
	}
	return search->second;
}

BOOL SWDamageMeter::isRun() {
	return _timer.isRun();
}

VOID SWDamageMeter::Suspend() {

	if (_historyMode) {
		Restore();
		_historyMode = FALSE;
	}

	_timer.Suspend();

	for (auto itr = _playerMetadata.begin(); itr != _playerMetadata.end(); itr++) {
		itr->second->MeterSuspended();
	}

	PLOTWINDOW.End();
}

VOID SWDamageMeter::Start() {

	if (_historyMode) {
		Restore();
		_historyMode = FALSE;
	}
	_timer.Run();
	PLOTWINDOW.Start();
}

VOID SWDamageMeter::Clear() {
	BOOL clearOwnerAndDB = true;
	if (!_historyMode) {
		if (_playerInfo.size() > 0) {
			unordered_map<UINT32, SW_PLAYER_METADATA*> newHistoryPlayerMeta;
			vector<SW_DB2_STRUCT*> newDB;
			for (auto itr = _playerMetadata.begin(); itr != _playerMetadata.end(); itr++) {
				itr->second->MeterReseted();

				// copy this time player to history
				for (auto itr2 = _playerInfo.begin(); itr2 != _playerInfo.end(); itr2++) {
					if ((*itr2)->GetID() == itr->first) {
						SW_PLAYER_METADATA* newMeta = new SW_PLAYER_METADATA;
						memcpy_s(newMeta, sizeof(SW_PLAYER_METADATA), itr->second, sizeof(SW_PLAYER_METADATA));
						newHistoryPlayerMeta.emplace(itr->first, newMeta);
						break;
					}
				}
			}
			for (auto itr2 = _dbInfo.begin(); itr2 != _dbInfo.end(); itr2++)
			{
				SW_DB2_STRUCT* newStruct = new SW_DB2_STRUCT;
				memcpy_s(newStruct, sizeof(SW_DB2_STRUCT), *itr2, sizeof(SW_DB2_STRUCT));
				newDB.push_back(newStruct);
			}

			BUFFMETER.EndAllBuff();

			HISTORY_DATA* hd = new HISTORY_DATA;
			hd->_playerHistory = _playerInfo;
			hd->_dbHistory = newDB;
			hd->_buffHistory = BUFFMETER.GetPlayerInfo();
			hd->_plotHistory = PLOTWINDOW.GetPlotInfo();
			hd->_playerMetadata = newHistoryPlayerMeta;
			hd->_combatIF = COMBATMETER.Get();

			HISTORY_INFO* pHI = new HISTORY_INFO;
			pHI->Setup(hd, DAMAGEMETER.GetWorldID(), DAMAGEMETER.GetTime(), DAMAGEMETER.GetMyID(), _testMode, nullptr, _realClearTime);

			if (_testMode)
			{
				ClearInfo(FALSE);
				pHI->Clear();
				delete pHI;
			}
			else {
				HISTORY.push_back(pHI);

				if (UIOPTION.isTeamTALF() && GetWorldID() == 22061) {
					ClearDB();

					for (auto itr = hd->_dbHistory.begin(); itr != hd->_dbHistory.end(); itr++) {
						SW_DB2_STRUCT* newDB = new SW_DB2_STRUCT;
						memcpy_s(newDB, sizeof(SW_DB2_STRUCT), *itr, sizeof(SW_DB2_STRUCT));
						_dbInfo.push_back(newDB);
					}

					clearOwnerAndDB = false;
				}
				ClearInfo(clearOwnerAndDB);
			}
		}
		else {
			// not player data
			COMBATMETER.Clear(TRUE);
		}
		_mazeEnd = FALSE;
	}
	else {
		Restore();
		_historyMode = FALSE;
	}
	_testMode = FALSE;
	_timer.Stop();
}

VOID SWDamageMeter::Toggle() {

	if (isRun())
		Suspend();
	else
		Start();
}

ULONG64 SWDamageMeter::GetTime() {

	if (!_historyMode)
		return _timer.GetTime();
	else
		return _historyTime;
}

VOID SWDamageMeter::SetMazeState(BOOL end) {
	_mazeEnd = end;
	if (end) {
		PLOTWINDOW.End();
		BUFFMETER.EndAllBuff();
	}
}

VOID SWDamageMeter::Restore() {

	if (!_historyMode)
		return;

	_currentHistoryId = -1;
	ClearInfo();
}

VOID SWDamageMeter::ClearInfo(BOOL clear)
{
	if (!_historyMode) {
		_playerInfo.clear();
	}

	if (clear) {
		_ownerInfo.clear();
		ClearDB();
	}

	_playerUseAwaken.clear();
	PLOTWINDOW.Clear();
	BUFFMETER.Clear();
	COMBATMETER.End();
	COMBATMETER.Clear();
	UTILLWINDOW.ClearCombatTemp();

	_aggroedId = 0;
	_currentHistoryId = -1;
	_historyHI = nullptr;
	_realClearTime = 0;
}

VOID SWDamageMeter::ClearDB()
{
	for (auto itr2 = _dbInfo.begin(); itr2 != _dbInfo.end(); itr2++)
	{
		delete (*itr2);
	}
	_dbInfo.clear();
}

VOID SWDamageMeter::SetHistory(LPVOID pHi) {

	if (pHi == nullptr)
		return;

	HISTORY_INFO* hi = (HISTORY_INFO*)pHi;

	if (!_historyMode) {
		Clear();
	}

	if (std::find(HISTORY.begin(), HISTORY.end(), hi) == HISTORY.end())
		return;

	_historyPlayerInfo = hi->_historyData->_playerHistory;
	_historyPlayerMetadata = hi->_historyData->_playerMetadata;
	_historyDbInfo = hi->_historyData->_dbHistory;
	BUFFMETER.SetPlayerInfo(hi->_historyData->_buffHistory);
	PLOTWINDOW.SetPlotInfo(hi->_historyData->_plotHistory);
	COMBATMETER.Set(hi->_historyData->_combatIF);

	_historyWorldID = hi->_worldID;
	_historyTime = hi->_time;
	_historyMyID = hi->_myID;
	_historyHI = hi;

	_historyMode = TRUE;
}

BOOL SWDamageMeter::isHistoryMode()
{
	return _historyMode;
}