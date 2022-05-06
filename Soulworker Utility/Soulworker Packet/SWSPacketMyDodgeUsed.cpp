#include "pch.h"
#include ".\Soulworker Packet\SWSPacketMyDodgeUsed.h"
#include ".\Damage Meter\Damage Meter.h"

SWSPacketMyDodgeUsed::SWSPacketMyDodgeUsed(SWSHEADER* swheader, BYTE* data) {
	_swheader = swheader;
	_data = data;
}

VOID SWSPacketMyDodgeUsed::Do() {
	SWPACKET_MYDODGEUSED* dodgeUsed = (SWPACKET_MYDODGEUSED*)(_data + sizeof(SWSHEADER));

	// check is it normal dodge (ground dodge) as rolling (roll after down) is also there
	if (normalDodgeList.find(dodgeUsed->_skillId) == normalDodgeList.end()) {
		return;
	}

	DAMAGEMETER.AddDodgeUsed(dodgeUsed->_playerId);

	return;
}

VOID SWSPacketMyDodgeUsed::Log() {
	return;
}

VOID SWSPacketMyDodgeUsed::Debug() {
	return;
}