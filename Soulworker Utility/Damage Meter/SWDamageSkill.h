#pragma once
#include "pch.h"
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"
#include ".\Damage Meter\MySQLite.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define DEBUG_DAMAGE_SKILL 0
#define SKILL_NAME_LEN 64

class SWDamageSkill : public MemoryPool<SWDamageSkill, 100> {
private:
	UINT32 _id;
	UINT64 _damage;
	UINT64 _critDamage;
	USHORT _hitCount;
	USHORT _critHitCount;
	USHORT _skillCasts;
	CHAR _name[SKILL_NAME_LEN];

public:
	SWDamageSkill() : _id(0), _damage(0), _critDamage(0), _hitCount(0), _critHitCount(0), _skillCasts(0) { }
	SWDamageSkill(UINT32 id, UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount);
	~SWDamageSkill() {}

	static BOOL SortFunction(SWDamageSkill* skillA, SWDamageSkill* skillB);

	UINT32 GetID();
	UINT64 GetDamage();
	UINT64 GetCritDamage();
	USHORT GetHitCount();
	USHORT GetCritHitCount();
	USHORT GetSkillUsed();
	CHAR* GetName();

	VOID SetNameFromDB();
	VOID AddDamage(UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount);

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tDamageSkill>>& vDamageSkill)
	{
		_tDamageSkillBuilder tdsb(fbb);

		tdsb.add__id(GetID());
		tdsb.add__damage(GetDamage());
		tdsb.add__critdamage(GetCritDamage());
		tdsb.add__hitcount(GetHitCount());
		tdsb.add__crithitcount(GetCritHitCount());
		tdsb.add__skillcasts(GetSkillUsed());

		vDamageSkill.push_back(tdsb.Finish());
	}

	VOID UnSerialization(const _tDamageSkill* tDamageSkill)
	{
		_id = tDamageSkill->_id();
		_damage = tDamageSkill->_damage();
		_critDamage = tDamageSkill->_critdamage();
		_hitCount = tDamageSkill->_hitcount();
		_critHitCount = tDamageSkill->_crithitcount();
		_skillCasts = tDamageSkill->_skillcasts();

		SetNameFromDB();
	}
};