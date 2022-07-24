#include "pch.h"
#include ".\Damage Meter\SWDamageSkill.h"

SWDamageSkill::SWDamageSkill(UINT32 id, UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount){
	_id = id;
	_damage = damage;
	_critDamage = critDamage;
	_hitCount = hitCount;
	_critHitCount = critHitCount;

	SetNameFromDB();

#if DEBUG_DAMAGE_SKILL == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("\t\t[SKILL] [ID = %d] [NAME = %s] [DMG = %llu] [cirDMG = %llu] [hitCount = %d] [cirtHitCount = %d]")), _id, _name,_damage, _critDamage, _hitCount, _critHitCount);
#endif
}

VOID SWDamageSkill::SetNameFromDB()
{
	ZeroMemory(_name, SKILL_NAME_LEN);
	SWDB.GetSkillName(_id, _name, SKILL_NAME_LEN);
}

BOOL SWDamageSkill::SortFunction(SWDamageSkill* skillA, SWDamageSkill* skillB) {
	return skillA->GetDamage() > skillB->GetDamage();
}

VOID SWDamageSkill::AddDamage(UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount) {
	_damage += damage;
	_critDamage += critDamage;
	_hitCount += hitCount;
	_critHitCount += critHitCount;

#if DEBUG_DAMAGE_SKILL == 1
	Log::WriteLog(const_cast<LPTSTR>(_T("\t\t[SKILL] [ID = %d] [NAME = %s] [DMG = %llu] [cirDMG = %llu] [hitCount = %d] [cirtHitCount = %d]")), _id, _name, _damage, _critDamage, _hitCount, _critHitCount);
#endif
}

UINT32 SWDamageSkill::GetID() {
	return _id;
}

UINT64 SWDamageSkill::GetDamage() {
	return _damage;
}

UINT64 SWDamageSkill::GetCritDamage() {
	return _critDamage;
}

USHORT SWDamageSkill::GetHitCount() {
	return _hitCount;
}

USHORT SWDamageSkill::GetCritHitCount() {
	return _critHitCount;
}

USHORT SWDamageSkill::GetSkillUsed()
{
	return _skillCasts;
}

CHAR* SWDamageSkill::GetName() {
	return _name;
}
