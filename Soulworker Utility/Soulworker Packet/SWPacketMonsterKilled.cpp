#include "pch.h"
#include ".\Soulworker Packet\SWPacketMonsterKilled.h"
#include ".\Damage Meter\Damage Meter.h"

SWPacketMonsterKilled::SWPacketMonsterKilled(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMonsterKilled::Do() {

	SWPACKETMONSTERKILLED* killed = (SWPACKETMONSTERKILLED*)(_data + sizeof(SWHEADER));
	DAMAGEMETER.SetAggro(killed->_killedId, 0xffffffff);
}

VOID SWPacketMonsterKilled::Log() {

}

VOID SWPacketMonsterKilled::Debug() {

	//SWPACKETGESTUREUSED* g_data = (SWPACKETGESTUREUSED*)(_data + sizeof(SWHEADER));
	//Log::MyLog(_T("[Gesture Used] [Player = %u] [Gesture = %u] [X=%f][Y=%f][Z=%f][Direct=%f][Screen=%f]"),
	//	g_data->_playerID, g_data->_gestureId, g_data->_locationX, g_data->_locationY, g_data->_locationZ
	//	, g_data->_direction, g_data->_screen);
}