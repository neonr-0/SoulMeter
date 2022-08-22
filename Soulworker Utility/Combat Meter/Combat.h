#pragma once
#include "pch.h"
#include ".\FlatBuffers\include\SW_HISTORY_.h"

using namespace std;
using namespace SoulMeterFBS::History;

enum CombatType
{
	PLAYER,
	MONSTER
};

enum CombatLogType
{
	CHANGED_STATS = 0,
	CHANGED_SPECIAL_STATS = 1,

	USED_SKILL = 10,

	GIVE_DAMAGE_MISS = 20,
	GIVE_DAMAGE_NORMAL = 21,
	GIVE_DAMAGE_CRIT = 22,

	TAKE_DAMAGE_MISS = 30,
	TAKE_DAMAGE_NORMAL = 31,
	TAKE_DAMAGE_CRIT = 32,

	BUFF_STARTED = 40,
	BUFF_END = 41,

	INVINCIBLE_SET = 50,
	INVINCIBLE_UNSET = 51
};

struct CombatLog
{
	SYSTEMTIME* _time;
	CombatLogType _type;
	DOUBLE _val1 = 0;
	DOUBLE _val2 = 0;
};

class Combat : public MemoryPool<Combat> {

private:
	UINT32 _id;
	UINT32 _type;
	vector<std::pair<ULONG64, CombatLog*>> _log;
	BOOL _isHistory = FALSE;

protected:
	Combat(const Combat& other) {}

public:
	Combat(UINT32 id, CombatType type) : _id(id), _type(type) {}

	VOID Insert(CombatLog* cl);
	VOID Clear();

	auto GetID()
	{
		return _id;
	}
	auto GetType()
	{
		return _type;
	}

	auto begin()
	{
		return _log.begin();
	}
	auto end()
	{
		return _log.end();
	}
	auto rbegin()
	{
		return _log.rbegin();
	}
	auto rend()
	{
		return _log.rend();
	}
	const size_t size()
	{
		return _log.size();
	}

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tCombat>>& vCombat)
	{
		std::vector<flatbuffers::Offset<_tCombatLog>> vCombatLog;
		for (auto itr = _log.begin(); itr != _log.end(); itr++)
		{
			CombatLog* pCombatLog = itr->second;
			SYSTEMTIME* pTime = pCombatLog->_time;
			auto sTime = _sTime(pTime->wYear, pTime->wMonth, pTime->wDayOfWeek, pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond, pTime->wMilliseconds);

			_tCombatLogBuilder tclb(fbb);
			tclb.add__log_type(pCombatLog->_type);
			tclb.add__time(&sTime);
			tclb.add__val1(pCombatLog->_val1);
			tclb.add__val2(pCombatLog->_val2);
			tclb.add__timestamp(itr->first);

			vCombatLog.push_back(tclb.Finish());
		}
		auto fcvCombatLog = fbb.CreateVector(vCombatLog);
		
		_tCombatBuilder tcb(fbb);
		tcb.add__id(_id);
		tcb.add__type(_type);
		tcb.add__combat_log(fcvCombatLog);

		vCombat.push_back(tcb.Finish());
	}
	VOID UnSerialization(const _tCombat* pCombat)
	{
		_isHistory = TRUE;

		_id = pCombat->_id();
		_type = pCombat->_type();

		for (auto itr = pCombat->_combat_log()->begin(); itr != pCombat->_combat_log()->end(); itr++)
		{
			SYSTEMTIME* pSysTime = new SYSTEMTIME;
			pSysTime->wYear = itr->_time()->year();
			pSysTime->wMonth = itr->_time()->month();
			pSysTime->wDayOfWeek = itr->_time()->day_of_week();
			pSysTime->wDay = itr->_time()->day();
			pSysTime->wHour = itr->_time()->hour();
			pSysTime->wMinute = itr->_time()->minute();
			pSysTime->wSecond = itr->_time()->second();
			pSysTime->wMilliseconds = itr->_time()->milliseconds();

			CombatLog* pCombatLog = new CombatLog;
			pCombatLog->_type = (CombatLogType)itr->_log_type();
			pCombatLog->_time = pSysTime;
			pCombatLog->_val1 = itr->_val1();
			pCombatLog->_val2 = itr->_val2();

			_log.push_back(std::pair(itr->_timestamp(), pCombatLog));
		}
	}
};

class CombatInterface : public MemoryPool<CombatInterface> {

private:
	unordered_map<UINT32, Combat*> _combat;
	BOOL _isHistory = FALSE;

protected:
	CombatInterface(const CombatInterface& other) {}

public:
	CombatInterface() {}

	VOID Insert(UINT32 id, CombatType type, CombatLog* cl);
	VOID Clear();

	auto begin()
	{
		return _combat.begin();
	}
	auto end()
	{
		return _combat.end();
	}
	const size_t size()
	{
		return _combat.size();
	}

	flatbuffers::Offset<_tCombatInterface> Serialization(flatbuffers::FlatBufferBuilder& fbb)
	{
		std::vector<flatbuffers::Offset<_tCombat>> vCombat;
		for (auto itr = _combat.begin(); itr != _combat.end(); itr++)
		{
			itr->second->Serialization(fbb, vCombat);
		}
		auto fcvCombat = fbb.CreateVector(vCombat);

		_tCombatInterfaceBuilder tcifb(fbb);
		tcifb.add__combat(fcvCombat);

		return tcifb.Finish();
	}
	VOID UnSerialization(const _tCombatInterface* pCombatInterface)
	{
		_isHistory = TRUE;

		for (auto itr = pCombatInterface->_combat()->begin(); itr != pCombatInterface->_combat()->end(); itr++)
		{
			Combat* pCombat = new Combat(itr->_id(), (CombatType)itr->_type());
			pCombat->UnSerialization(*itr);

			_combat[itr->_id()] = pCombat;
		}
	}
};