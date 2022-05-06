#include "pch.h"
#include ".\Soulworker Packet\SWPacketMazeEnd.h"
#include ".\Damage Meter\Damage Meter.h"

SWPacketMazeEnd::SWPacketMazeEnd(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMazeEnd::Do() {
	DAMAGEMETER.SetMazeState(TRUE);
	DAMAGEMETER.Suspend();
}

VOID SWPacketMazeEnd::Log() {

}

VOID SWPacketMazeEnd::Debug() {
	//Log::WriteLog(const_cast<LPTSTR>(_T("[TEST] [Maze End]")));

	//Log::MyLog(_T("Maze End\n"));
	//for (int i = sizeof(SWHEADER); i < _swheader->_size; i++)
	//	Log::MyLog(_T("%02x "), _data[i]);
	//Log::MyLog(_T("\n"));
}