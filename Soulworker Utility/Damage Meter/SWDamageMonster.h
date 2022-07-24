#pragma once
#include ".\Damage Meter\SWDamageSkill.h"
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define DEBUG_DAMAGE_MONSTER 0
#define MONSTER_NAME_LEN 64

class SWDamageMonster : public MemoryPool<SWDamageMonster, 100> {
private:
	UINT32 _id;
	//USHORT _db1;
	UINT32 _db2;
	UINT64 _damage;
	UINT64 _critDamage;
	USHORT _hitCount;
	USHORT _critHitCount;
	CHAR _name[MONSTER_NAME_LEN];
	INT32 _type;

	vector<SWDamageSkill*> _skillinfo;

	VOID InsertSkillInfo(UINT32 skillID, UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount);
	VOID Sort();

public:
	SWDamageMonster(UINT32 id, UINT32 db2, UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount, UINT32 skillID);
	SWDamageMonster() : _type(-1), _id(0), _db2(0), _damage(0), _critDamage(0), _hitCount(0), _critHitCount(0) { }
	~SWDamageMonster();

	static BOOL SortFunction(SWDamageMonster* monsterA, SWDamageMonster* monsterB);

	UINT32 GetID();
	INT32 GetType();
	//USHORT GetDB1();
	UINT32 GetDB2();
	UINT64 GetDamage();
	UINT64 GetCritDamage();
	USHORT GetHitCount();
	USHORT GetCritHitCount();
	UINT64 GetSkillTotalDamage();
	const CHAR* GetName();

	vector<SWDamageSkill*>::const_iterator begin();
	vector<SWDamageSkill*>::const_iterator end();
	const SIZE_T size();

	VOID SetNameFromDB();
	VOID AddDamage(UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount, UINT32 skillID);

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tDamageMonster>>& vDamageMonster)
	{
		// _tDamageSkill
		std::vector<flatbuffers::Offset<_tDamageSkill>> vSkillInfo;

		for (auto skill_itr = begin(); skill_itr != end(); skill_itr++)
			(*skill_itr)->Serialization(fbb, vSkillInfo);
		auto fcvSkillInfo = fbb.CreateVector(vSkillInfo);

		// _tDamageMonster
		_tDamageMonsterBuilder tdmb(fbb);

		tdmb.add__id(GetID());
		tdmb.add__db2(GetDB2());
		tdmb.add__damage(GetDamage());
		tdmb.add__critdamage(GetCritDamage());
		tdmb.add__hitcount(GetHitCount());
		tdmb.add__crithitcount(GetCritHitCount());
		tdmb.add__type(GetType());
		tdmb.add__skillinfo(fcvSkillInfo);

		vDamageMonster.push_back(tdmb.Finish());
	}
	VOID UnSerialization(const _tDamageMonster* tDamageMonster)
	{
		// _tDamageSkill
		for (auto itr = tDamageMonster->_skillinfo()->begin(); itr != tDamageMonster->_skillinfo()->end(); itr++)
		{
			SWDamageSkill* newDamageSkill = new SWDamageSkill;
			newDamageSkill->UnSerialization(*itr);

			_skillinfo.push_back(newDamageSkill);
		}

		// _tDamageMonster
		_id = tDamageMonster->_id();
		_db2 = tDamageMonster->_db2();
		_damage = tDamageMonster->_damage();
		_critDamage = tDamageMonster->_critdamage();
		_hitCount = tDamageMonster->_hitcount();
		_critHitCount = tDamageMonster->_crithitcount();
		_type = tDamageMonster->_type();

		SetNameFromDB();
	}
};