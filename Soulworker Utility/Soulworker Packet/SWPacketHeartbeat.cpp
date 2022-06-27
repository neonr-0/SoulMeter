#include "pch.h"
#include ".\Soulworker Packet\SWPacketHeartbeat.h"
#include ".\UI\PlayerTable.h"

SWPacketHeartbeat::SWPacketHeartbeat(SWHEADER* swheader, BYTE* data, UINT64 ts) : SWPacket(swheader, data), _ts(ts) {
}

VOID SWPacketHeartbeat::Do() {
	
	SWPACKETHEARTBEAT* hbData = (SWPACKETHEARTBEAT*)(_data + sizeof(SWHEADER));

	if (PLAYERTABLE._tick == hbData->_tick) {
		PLAYERTABLE._ping = _ts - PLAYERTABLE._lastSendTimestamp;
	}
}

VOID SWPacketHeartbeat::Log() {

}

VOID SWPacketHeartbeat::Debug() {

	SWPACKETHEARTBEAT* hbData = (SWPACKETHEARTBEAT*)(_data + sizeof(SWHEADER));
	//Log::WriteLog(_T("recv tick : %u / "), hbData->_tick);
	//for (int i = 0; i < sizeof(hbData->_unknown01); i++)
	//	Log::MyLog(_T("%02x "), hbData->_unknown01[i]);
	//Log::MyLog(_T("\n"));
}