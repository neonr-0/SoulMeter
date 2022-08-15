#pragma once
#include ".\Damage Meter\SWDamagePlayer.h"
#include ".\Buff Meter\Buff Meter.h"
#include ".\UI\PlotWindow.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"
#include ".\Combat Meter\Combat.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define HISTORY SWDamageMeterHistory::getInstance()
#define HISTORY_SIZE 50

struct HISTORY_DATA
{
	vector<SWDamagePlayer*> _playerHistory;
	vector<SW_DB2_STRUCT*> _dbHistory;
	vector<PLAYERBUF*> _buffHistory;
	PlotInfo* _plotHistory = nullptr;
	unordered_map<UINT32, SWDamageMeter::SW_PLAYER_METADATA*> _playerMetadata;
	string _extInfo;
	CombatInterface* _combatIF = nullptr;
};

typedef struct _HISTORYINFO {
public:
	UINT32 _worldID;
	SYSTEMTIME* _saveTime;
	ULONG64 _time;
	HISTORY_DATA* _historyData;
	UINT32 _myID;
	UINT32 _realClearTime;
	BOOL _isSaveData = FALSE;

	VOID Setup(HISTORY_DATA* historyData, UINT32 worldID, ULONG64 time, UINT32 myID, BOOL isSaveData = FALSE, SYSTEMTIME* saveTime = nullptr, UINT32 realClearTime = 0);
	VOID Clear();

	flatbuffers::Offset<_tHistory> Serialization(flatbuffers::FlatBufferBuilder& fbb, HISTORY_DATA* historyData);
}HISTORY_INFO;

class SWDamageMeterHistory : public Singleton<SWDamageMeterHistory> {
private:
	vector<LPVOID> _historys;
	INT _curIndex = 0;
	
	mutex _mutex;

	BOOL _stopAddHistory = FALSE;

public:
	SWDamageMeterHistory() : _curIndex(0) {}
	~SWDamageMeterHistory();

	VOID push_back(HISTORY_INFO* pHi);
	VOID ClearHistory(HISTORY_INFO* pHI = nullptr, BOOL deleteFirst = TRUE);
	VOID ClearAll();
	VOID ClearVector();

	VOID UnSerialization(const _tHistory* pHistory);

	INT GetCurrentIndex()
	{
		return _curIndex;
	}

	VOID GetLock()
	{
		_mutex.lock();
	}

	VOID FreeLock()
	{
		_mutex.unlock();
	}

	auto begin()
	{
		return _historys.begin();
	}

	auto end()
	{
		return _historys.end();
	}

	auto rbegin()
	{
		return _historys.rbegin();
	}

	auto rend()
	{
		return _historys.rend();
	}

	const size_t size()
	{
		return _historys.size();
	}

	BOOL isStop()
	{
		return _stopAddHistory;
	}
};