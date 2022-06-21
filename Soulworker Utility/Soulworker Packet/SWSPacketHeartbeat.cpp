#include "pch.h"
#include ".\Soulworker Packet\SWSPacketHeartbeat.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\UI\PlayerTable.h"

SWSPacketHeartbeat::SWSPacketHeartbeat(SWSHEADER* swheader, BYTE* data) {
	_swheader = swheader;
	_data = data;
}

VOID SWSPacketHeartbeat::Do() {
	PLAYERTABLE._lastSendTimestamp = GetCurrentTimeStamp();
	return;
}

VOID SWSPacketHeartbeat::Log() {
	return;
}

VOID SWSPacketHeartbeat::Debug() {
	_SWSPACKET_HEARTBEAT* hbData = (_SWSPACKET_HEARTBEAT*)(_data + sizeof(SWSHEADER));
	//Log::WriteLog(_T("send tick : %u / "), hbData->_tick);
	return;
}