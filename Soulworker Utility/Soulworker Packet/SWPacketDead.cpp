#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Soulworker Packet\SWPacketDead.h"
#include ".\Damage Meter\Damage Meter.h"


SWPacketDead::SWPacketDead(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketDead::Do() {
	SWPACKET_DEAD* stat_header = (SWPACKET_DEAD*)(_data + sizeof(SWHEADER));

	UINT32 playerId = stat_header->_playerid;
	if (DAMAGEMETER.CheckPlayer(playerId)) {
		DAMAGEMETER.AddDeath(playerId);
	}
}

VOID SWPacketDead::Log() {

}

VOID SWPacketDead::Debug() {

}