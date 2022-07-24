#pragma once
#include ".\Damage Meter\SWDamagePlayer.h"
#include ".\Buff Meter\Buff Meter.h"
#include ".\UI\PlotWindow.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define HISTORY SWDamageMeterHistory::getInstance()
#define HISTORY_SIZE 50

struct HISTORY_DATA
{
	vector<SWDamagePlayer*> _playerHistory;
	vector<SW_DB2_STRUCT*> _dbHistory;
	vector<PLAYERBUF*> _buffHistory;
	PlotInfo* _plotHistory;
	unordered_map<UINT32, SWDamageMeter::SW_PLAYER_METADATA*> _playerMetadata;
	string _extInfo;
};

typedef struct _HISTORYINFO {
public:
	UINT32 _worldID;
	SYSTEMTIME* _saveTime;
	ULONG64 _time;
	HISTORY_DATA* _historyData;
	UINT32 _myID;
	BOOL _isSaveData = FALSE;

	VOID Setup(HISTORY_DATA* historyData, UINT32 worldID, ULONG64 time, UINT32 myID, BOOL isSaveData = FALSE, SYSTEMTIME* saveTime = nullptr);
	VOID Clear();

	flatbuffers::Offset<_tHistory> Serialization(flatbuffers::FlatBufferBuilder& fbb, HISTORY_DATA* historyData);
}HISTORY_INFO;

class SWDamageMeterHistory : public Singleton<SWDamageMeterHistory> {
private:
	vector<LPVOID> _historys;
	INT _curIndex = 0;
	
	recursive_mutex _mutex;

	BOOL _stopAddHistory = FALSE;

public:
	SWDamageMeterHistory() : _curIndex(0) {}
	~SWDamageMeterHistory();

	VOID push_back(HISTORY_INFO* pHi);
	VOID ClearHistory(HISTORY_INFO* pHI = nullptr, BOOL deleteFirst = TRUE);

	VOID UnSerialization(const _tHistory* pHistory);

	INT GetCurrentIndex()
	{
		return _curIndex;
	}

	recursive_mutex* GetMutex()
	{
		return &_mutex;
	}

	vector<LPVOID>* GetVector()
	{
		return &_historys;
	}

	BOOL isStop()
	{
		return _stopAddHistory;
	}
};