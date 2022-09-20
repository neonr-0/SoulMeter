#pragma once
#include ".\Damage Meter\SWDamageMonster.h"
#include ".\Soulworker Packet\PacketInfo.h"
#include ".\Damage Meter\MonsterList.h"
#include <unordered_set>
#include <map>
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"
using namespace std;
using namespace SoulMeterFBS::History;

#define DEBUG_DAMAGE_PLAYER 0

struct SkillCount {
	USHORT _count;
	USHORT _in_full_ab_count;
};

class SWDamagePlayer : public MemoryPool<SWDamagePlayer, 30> {
private:
	UINT32 _id;
	UINT64 _damage;
	UINT64 _soulstoneDamage;
	USHORT _hitCount;
	USHORT _critHitCount;
	USHORT _maxCombo;
	USHORT _hitCountForCritRate;
	USHORT _critHitCountForCritRate;
	USHORT _missCount;
	USHORT _soulstoneCount;
	UINT64 _damageForSoulstone;
	UINT64 _soulstoneDamageForSoulstone;
	USHORT _hitCountForArmorBreak;
	FLOAT _armorBreak;
	FLOAT _sg;
	FLOAT _stamina;
	FLOAT _sv;
	FLOAT _attackSpeed;
	FLOAT _partialDamage;
	USHORT _getHitAll;
	USHORT _getHit;
	USHORT _getHitBS;
	USHORT _getHitMissed;
	USHORT _getHitMissedReal;
	FLOAT _enlightenSum;
	USHORT _gigaEnlightenProc;
	USHORT _teraEnlightenProc;
	USHORT _skillCounts = 0;
	USHORT _dodgeCounts = 0;
	USHORT _deathCounts = 0;

	DOUBLE _historyAvgAB = 0.0;
	DOUBLE _historyAvgBD = 0.0;

	DOUBLE _historygear90 = 0.0;
	DOUBLE _historygear50 = 0.0;
	DOUBLE _historyacc01 = 0.0;
	DOUBLE _historyacc02 = 0.0;

	DOUBLE _historyLosedHP = 0.0;
	BYTE _JqStack = 0;

	DOUBLE _historyABTime = 0;

	DOUBLE _historyAggroTime = 0;

	DOUBLE _historyASTime = 0;
	DOUBLE _historyAvgAS = 0.0;

	vector<SWDamageMonster*> _monsterInfo;
	VOID InsertMonsterInfo(UINT32 monsterID, UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount, UINT32 skillID);
	VOID Sort();

protected:
	

public:
	SWDamagePlayer() : _id(0), _damage(0), _soulstoneDamage(0), _hitCount(0), _critHitCount(0), _maxCombo(0),
		_hitCountForCritRate(0), _critHitCountForCritRate(0), _soulstoneCount(0), _hitCountForArmorBreak(0),
		_armorBreak(0), _sg(0), _stamina(0), _sv(0), _attackSpeed(0), _partialDamage(0.0f) { }
	SWDamagePlayer(UINT32 id, UINT64 damage, UINT64 soulstoneDamage, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID);
	SWDamagePlayer(UINT32 id);
	~SWDamagePlayer();

	map<UINT32, SkillCount*> skillCounts;

	static BOOL SortFunction(SWDamagePlayer* playerA, SWDamagePlayer* playerB);

	UINT32 GetID();
	UINT64 GetDamage();
	UINT64 GetSoulstoneDamage();
	USHORT GetHitCount();
	USHORT GetCritHitCount();
	USHORT GetMaxCombo();
	UINT64 GetMonsterTotalDamage();
	USHORT GetHitCountForCritRate();
	USHORT GetCritHitCountForCritRate();
	USHORT GetMissCount();
	USHORT GetSoulstoneCount();
	UINT64 GetDamageForSoulstone();
	UINT64 GetSoulStoneDamageForSoulstone();
	USHORT GetGetHitAll();
	USHORT GetGetHit();
	USHORT GetGetHitBS();
	USHORT GetGetHitMissed();
	USHORT GetGetHitMissedReal();
	FLOAT GetEnlightenSum();
	USHORT GetGigaEnlighten();
	USHORT GetTeraEnlighten();
	USHORT GetSkillUsed();
	USHORT GetDodgeUsed();
	USHORT GetDeathCount();

	DOUBLE GetHistoryABTime();
	VOID SetHistoryABTime(DOUBLE historyABTime);

	VOID SetHistoryAvgAB(DOUBLE historyAvgAB);
	DOUBLE GetHistoryAvgAB();

	VOID SetHistoryAvgBD(DOUBLE historyAvgBD);
	DOUBLE GetHistoryAvgBD();

	VOID SetHistoryAggroTime(DOUBLE t);
	DOUBLE GetHistoryAggroTime();

	DOUBLE GetHistoryASTime();
	VOID SetHistoryASTime(DOUBLE d);

	VOID SetHistoryAvgAS(DOUBLE d);
	DOUBLE GetHistoryAvgAS();

	VOID setHistoryBS(int type, DOUBLE value);
	DOUBLE GetHistoryBS(int type);

	VOID setHistoryLosedHP(DOUBLE losedHP);
	DOUBLE GetHistoryLosedHP();

	VOID SetJqStack(BYTE stack);
	BYTE GetJqStack();

	VOID AddGetDamage(UINT64 totalDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, UINT32 monsterID, UINT32 skillID);
	VOID AddEnlighten(FLOAT value);
	VOID AddSkillUsed(UINT32 skillId);
	VOID AddDodgeUsed();
	VOID AddDeathCount();

	vector<SWDamageMonster*>::const_iterator GetMonsterInfo(UINT id);
	vector<SWDamageMonster*>::const_iterator begin();
	vector<SWDamageMonster*>::const_iterator end();
	const SIZE_T size();

	VOID AddDamage(UINT64 totalDMG, UINT64 soulstoneDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID);

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tDamagePlayer>>& vDamagePlayer)
	{
		// _tDamageMonster
		std::vector<flatbuffers::Offset<_tDamageMonster>> vDamageMonster;
		for (auto itr = _monsterInfo.begin(); itr != _monsterInfo.end(); itr++)
			(*itr)->Serialization(fbb, vDamageMonster);

		// _tSkillCount
		std::vector<flatbuffers::Offset<_tSkillCount>> vSkillCount;
		for (auto itr = skillCounts.begin(); itr != skillCounts.end(); itr++) {
			_tSkillCountBuilder tscb(fbb);

			tscb.add__id(itr->first);
			tscb.add__count(itr->second->_count);
			tscb.add__in_full_ab_count(itr->second->_in_full_ab_count);

			vSkillCount.push_back(tscb.Finish());
		}

		auto fcvDamageMonster = fbb.CreateVector(vDamageMonster);
		auto fcvSkillCount = fbb.CreateVector(vSkillCount);

		// _tDamagePlayer
		_tDamagePlayerBuilder tdpb(fbb);

		tdpb.add__id(_id);
		tdpb.add__damage(_damage);
		tdpb.add__soulstonedamage(_soulstoneDamage);
		tdpb.add__hitcount(_hitCount);
		tdpb.add__crithitcount(_critHitCount);

		tdpb.add__maxcombo(_maxCombo);
		tdpb.add__hitcountforcritrate(_hitCountForCritRate);
		tdpb.add__crithitcountforcritrate(_critHitCountForCritRate);
		tdpb.add__misscount(_missCount);
		tdpb.add__soulstonecount(_soulstoneCount);

		tdpb.add__damageforsoulstone(_damageForSoulstone);
		tdpb.add__soulstonedamageforsoulstone(_soulstoneDamageForSoulstone);
		tdpb.add__hitcountforarmorbreak(_hitCountForArmorBreak);
		tdpb.add__armorbreak(_armorBreak);
		tdpb.add__sg(_sg);

		tdpb.add__stamina(_stamina);
		tdpb.add__sv(_sv);
		tdpb.add__attackspeed(_attackSpeed);
		tdpb.add__partialdamage(_partialDamage);
		tdpb.add__gethitall(_getHitAll);

		tdpb.add__gethit(_getHit);
		tdpb.add__gethitbs(_getHitBS);
		tdpb.add__gethitmissed(_getHitMissed);
		tdpb.add__gethitmissedreal(_getHitMissedReal);
		tdpb.add__enlightensum(_enlightenSum);

		tdpb.add__gigaenlightenproc(_gigaEnlightenProc);
		tdpb.add__teraenlightenproc(_teraEnlightenProc);
		tdpb.add__skillcounts(_skillCounts);
		tdpb.add__dodgecounts(_dodgeCounts);
		tdpb.add__deathcounts(_deathCounts);

		tdpb.add__historyavgab(_historyAvgAB);
		tdpb.add__historyavgbd(_historyAvgBD);
		tdpb.add__historygear90(_historygear90);
		tdpb.add__historygear50(_historygear50);
		tdpb.add__historyacc01(_historyacc01);

		tdpb.add__historyacc02(_historyacc02);
		tdpb.add__historylosedhp(_historyLosedHP);
		tdpb.add__jqstack(_JqStack);
		tdpb.add__historyabtime(_historyABTime);

		tdpb.add__monsterinfo(fcvDamageMonster);
		tdpb.add__skillcount(fcvSkillCount);

		tdpb.add__historyaggrotime(_historyAggroTime);

		tdpb.add__historyavgas(_historyAvgAS);
		tdpb.add__historyastime(_historyASTime);
		
		vDamagePlayer.push_back(tdpb.Finish());
	}
	VOID UnSerialization(const _tDamagePlayer* tDamagePlayer)
	{
		// _tDamageMonster
		for (auto itr = tDamagePlayer->_monsterinfo()->begin(); itr != tDamagePlayer->_monsterinfo()->end(); itr++)
		{
			SWDamageMonster* newDamageMonster = new SWDamageMonster;
			newDamageMonster->UnSerialization(*itr);

			_monsterInfo.push_back(newDamageMonster);
		}

		// _tSkillCount
		for (auto itr = tDamagePlayer->_skillcount()->begin(); itr != tDamagePlayer->_skillcount()->end(); itr++)
		{
			SkillCount* newSkillCount = new SkillCount;
			newSkillCount->_count = itr->_count();
			newSkillCount->_in_full_ab_count = itr->_in_full_ab_count();

			skillCounts[itr->_id()] = newSkillCount;
		}

		_id = tDamagePlayer->_id();
		_damage = tDamagePlayer->_damage();
		_soulstoneDamage = tDamagePlayer->_soulstonedamage();
		_hitCount = tDamagePlayer->_hitcount();
		_critHitCount = tDamagePlayer->_crithitcount();

		_maxCombo = tDamagePlayer->_maxcombo();
		_hitCountForCritRate = tDamagePlayer->_hitcountforcritrate();
		_critHitCountForCritRate = tDamagePlayer->_crithitcountforcritrate();
		_missCount = tDamagePlayer->_misscount();
		_soulstoneCount = tDamagePlayer->_soulstonecount();

		_damageForSoulstone = tDamagePlayer->_damageforsoulstone();
		_soulstoneDamageForSoulstone = tDamagePlayer->_soulstonedamageforsoulstone();
		_hitCountForArmorBreak = tDamagePlayer->_hitcountforarmorbreak();
		_armorBreak = tDamagePlayer->_armorbreak();
		_sg = tDamagePlayer->_sg();

		_stamina = tDamagePlayer->_stamina();
		_sv = tDamagePlayer->_sv();
		_attackSpeed = tDamagePlayer->_attackspeed();
		_partialDamage = tDamagePlayer->_partialdamage();
		_getHitAll = tDamagePlayer->_gethitall();

		_getHit = tDamagePlayer->_gethit();
		_getHitBS = tDamagePlayer->_gethitbs();
		_getHitMissed = tDamagePlayer->_gethitmissed();
		_getHitMissedReal = tDamagePlayer->_gethitmissedreal();
		_enlightenSum = tDamagePlayer->_enlightensum();

		_gigaEnlightenProc = tDamagePlayer->_gigaenlightenproc();
		_teraEnlightenProc = tDamagePlayer->_teraenlightenproc();
		_skillCounts = tDamagePlayer->_skillcounts();
		_dodgeCounts = tDamagePlayer->_dodgecounts();
		_deathCounts = tDamagePlayer->_deathcounts();

		_historyAvgAB = tDamagePlayer->_historyavgab();
		_historyAvgBD = tDamagePlayer->_historyavgbd();
		_historygear90 = tDamagePlayer->_historygear90();
		_historygear50 = tDamagePlayer->_historygear50();
		_historyacc01 = tDamagePlayer->_historyacc01();

		_historyacc02 = tDamagePlayer->_historyacc02();
		_historyLosedHP = tDamagePlayer->_historylosedhp();
		_JqStack = tDamagePlayer->_jqstack();
		_historyABTime = tDamagePlayer->_historyabtime();

		_historyAggroTime = tDamagePlayer->_historyaggrotime();

		_historyAvgAS = tDamagePlayer->_historyavgas();
		_historyASTime = tDamagePlayer->_historyastime();
	}
};