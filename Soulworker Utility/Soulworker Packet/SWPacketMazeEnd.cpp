#include "pch.h"
#include ".\Soulworker Packet\SWPacketMazeEnd.h"
#include ".\Damage Meter\Damage Meter.h"

SWPacketMazeEnd::SWPacketMazeEnd(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMazeEnd::Do() 
{
	SWPACKETCHATMAZEEND* pMazeEnd = (SWPACKETCHATMAZEEND*)(_data + sizeof(SWHEADER));

	DAMAGEMETER.SetMazeState(TRUE);
	DAMAGEMETER.Suspend();
	DAMAGEMETER.SetRealClearTime(pMazeEnd->_clearTime);
}

VOID SWPacketMazeEnd::Log() {

}

VOID SWPacketMazeEnd::Debug() {
	/*SWPACKETCHATMAZEEND* pMazeEnd = (SWPACKETCHATMAZEEND*)(_data + sizeof(SWHEADER));
	Log::WriteLogA("[SWPacketMazeEnd] rank = %d, playScore = %u, clearScore = %u, clearTime = %u",
		pMazeEnd->_rank, pMazeEnd->_playScore, pMazeEnd->_clearScore, pMazeEnd->_clearTime);*/
}