#include "pch.h"
#include ".\Soulworker Packet\SWPacketGestureUsed.h"


SWPacketGestureUsed::SWPacketGestureUsed(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketGestureUsed::Do() {

	SWPACKETGESTUREUSED* gesture_data = (SWPACKETGESTUREUSED*)(_data + sizeof(SWHEADER));
}

VOID SWPacketGestureUsed::Log() {

}

VOID SWPacketGestureUsed::Debug() {

	//SWPACKETGESTUREUSED* g_data = (SWPACKETGESTUREUSED*)(_data + sizeof(SWHEADER));
	//Log::MyLog(_T("[Gesture Used] [Player = %u] [Gesture = %u] [X=%f][Y=%f][Z=%f][Direct=%f][Screen=%f]"),
	//	g_data->_playerID, g_data->_gestureId, g_data->_locationX, g_data->_locationY, g_data->_locationZ
	//	, g_data->_direction, g_data->_screen);
}