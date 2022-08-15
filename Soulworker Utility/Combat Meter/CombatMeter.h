#pragma once
#include "pch.h"
#include ".\Combat Meter\Combat.h"
#include ".\FlatBuffers\include\SW_HISTORY_.h"
#include ".\Soulworker Packet\PacketInfo.h"

using namespace SoulMeterFBS::History;

#define COMBATMETER CombatMeter::getInstance()

class CombatMeter : public Singleton<CombatMeter> {
private:
	BOOL _historyMode = false;
	BOOL _isEnd = false;

	mutex _mutex;

	CombatInterface* _ci = nullptr;

public:
	CombatMeter() {}
	~CombatMeter()
	{
		BOOL a = _mutex.try_lock();
		_mutex.unlock();
	}

	auto Get()
	{
		return _ci;
	}
	VOID Set(CombatInterface* ci)
	{
		GetLock();
		{
			if (!_historyMode && _ci != nullptr)
			{
				_ci->Clear();
				delete _ci;
			}

			_ci = ci;
			_historyMode = true;
			FreeLock();
		}
	}
	VOID Clear(BOOL realClear = FALSE)
	{
		GetLock();
		{
			if (!_historyMode && realClear && _ci != nullptr)
			{
				_ci->Clear();
				delete _ci;
			}
			_ci = nullptr;
			_historyMode = false;
			_isEnd = false;
			FreeLock();
		}
	}
	VOID End()
	{
		GetLock();
		{
			_isEnd = TRUE;
			FreeLock();
		}
	}

	VOID Insert(UINT32 id, CombatType type, CombatLog* cl);

	string ConvertCombatLogVal(CombatLog* pCombatLog, CombatType type);
	string GetName(Combat* pCombat);
	CombatLogType ConvertDamageTypeForGiveDamage(SWPACKETDAMAGE_DAMAGETYPE damageType);
	CombatLogType ConvertDamageTypeForTakeDamage(SWPACKETDAMAGE_DAMAGETYPE damageType);

	VOID GetLock()
	{
		_mutex.lock();
	}
	VOID FreeLock()
	{
		_mutex.unlock();
	}
};