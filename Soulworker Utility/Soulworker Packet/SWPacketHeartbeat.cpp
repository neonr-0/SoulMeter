#include "pch.h"
#include ".\Soulworker Packet\SWPacketHeartbeat.h"
#include ".\UI\PlayerTable.h";
#include ".\UI\PlayerTable.h";

SWPacketHeartbeat::SWPacketHeartbeat(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketHeartbeat::Do() {
	PLAYERTABLE._ping = GetCurrentTimeStamp() - PLAYERTABLE._lastSendTimestamp;
}

VOID SWPacketHeartbeat::Log() {

}

VOID SWPacketHeartbeat::Debug() {

	SWPACKETHEARTBEAT* hbData = (SWPACKETHEARTBEAT*)(_data + sizeof(SWHEADER));
	printf("OP : %04x\tsize : %04x\n", _swheader->_op, _swheader->_size);
	for (int i = 0; i < _swheader->_size; i++)
		printf(const_cast<CHAR*>("%02x "), _data[i]);
	printf(const_cast<CHAR*>("\n"));
	//Log::MyLog(_T("time : %u / "), hbData->_millisecond);
	//for (int i = 0; i < sizeof(hbData->_unknown01); i++)
	//	Log::MyLog(_T("%02x "), hbData->_unknown01[i]);
	//Log::MyLog(_T("\n"));
}