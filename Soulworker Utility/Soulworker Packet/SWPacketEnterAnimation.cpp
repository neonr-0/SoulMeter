#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketEnterAnimation.h"
#include ".\Combat Meter\CombatMeter.h"

SWPacketEnterAnimation::SWPacketEnterAnimation(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketEnterAnimation::Do() {

	SWPACKETENTERANIMATION* packet = (SWPACKETENTERANIMATION*)(_data + sizeof(SWHEADER));

	switch (packet->_type1)
	{
	case 0:
	{
		// set invincible
		if (packet->_type2 >= 0 && packet->_type2 <= 1)
		{
			CombatLog* pCombatLog = new CombatLog;
			pCombatLog->_type = (packet->_type2 == 1 ? CombatLogType::INVINCIBLE_SET : CombatLogType::INVINCIBLE_UNSET);
			pCombatLog->_val1 = static_cast<DOUBLE>(packet->_player_id);
			COMBATMETER.Insert(packet->_player_id, CombatType::PLAYER, pCombatLog);
		}
		break;
	}
	case 1:
		// unk
		break;
	case 2:
		// unk
		break;
	}
}

VOID SWPacketEnterAnimation::Log() {

}

VOID SWPacketEnterAnimation::Debug() {
}