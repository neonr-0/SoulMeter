#include "pch.h"
#include ".\Soulworker Packet\SWSPacketMySkillUsed.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Combat Meter\CombatMeter.h"

SWSPacketMySkillUsed::SWSPacketMySkillUsed(SWSHEADER* swheader, BYTE* data) {
	_swheader = swheader;
	_data = data;
}

VOID SWSPacketMySkillUsed::Do() {
	SWPACKET_MYSKILLUSED* skillUsed = (SWPACKET_MYSKILLUSED*)(_data + sizeof(SWSHEADER));

	DAMAGEMETER.AddSkillUsed(skillUsed->_playerId, skillUsed->_skillId);

	CombatLog* pCombatLog = new CombatLog;
	pCombatLog->_type = CombatLogType::USED_SKILL;
	pCombatLog->_val1 = skillUsed->_skillId;
	COMBATMETER.Insert(skillUsed->_playerId, CombatType::PLAYER, pCombatLog);
	return;
}

VOID SWSPacketMySkillUsed::Log() {
	return;
}

VOID SWSPacketMySkillUsed::Debug() {
	return;
}