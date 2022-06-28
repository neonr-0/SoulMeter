#pragma once
#include ".\Damage Meter\SWDamagePlayer.h"
#include ".\Buff Meter\Buff Meter.h"
#include ".\UI\PlotWindow.h"
#include ".\Damage Meter\Damage Meter.h"

#define HISTORY SWDamageMeterHistory::getInstance()
#define HISTORY_SIZE 10

struct HISTORY_DATA
{
	vector<SWDamagePlayer*> _playerHistory;
	vector<SW_OWNER_ID_STRUCT*> _ownerHistory;
	vector<SW_DB2_STRUCT*> _dbHistory;
	vector<PLAYERBUF*> _buffHistory;
	PlotInfo* _plotHistory;
};

typedef struct _HISTORYINFO {
public:
	UINT32 _worldID;
	SYSTEMTIME _saveTime;
	ULONG64 _time;
	HISTORY_DATA* _historyData;

	VOID Setup(HISTORY_DATA* historyData, UINT32 worldID, ULONG64 time);
	VOID Clear();
}HISTORY_INFO;

class SWDamageMeterHistory : public Singleton<SWDamageMeterHistory> {
private:
	HISTORY_INFO _historys[HISTORY_SIZE];
	INT _curIndex = 0;
	
	VOID ClearHistory(INT index);

public:
	SWDamageMeterHistory() : _curIndex(0) {}
	~SWDamageMeterHistory();

	VOID push_back(HISTORY_DATA* historyData);
	SIZE_T size();
	const HISTORY_INFO& operator[](INT index);
	INT GetCurrentIndex()
	{
		return _curIndex;
	}
};