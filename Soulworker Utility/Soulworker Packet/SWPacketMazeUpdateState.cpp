#include "pch.h"
#include ".\Soulworker Packet\SWPacketMazeUpdateState.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\UI\Option.h"
#include ".\Damage Meter\MapList.h"

SWPacketMazeUpdateState::SWPacketMazeUpdateState(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMazeUpdateState::Do() {

	SWPacketMazeUpdateStatePacket* packet = (SWPacketMazeUpdateStatePacket*)(_data + sizeof(SWHEADER));
	
	if (!UIOPTION.isSoloRankMode() || rankMap.find((UINT32)DAMAGEMETER.GetWorldID()) == rankMap.end())
		return;

	switch (packet->_stateID)
	{
		// Maze waiting(can't use skill)
	case 0x00:
		break;
		// Maze start
	case 0x01:
		DAMAGEMETER.SetMazeState(FALSE);
		DAMAGEMETER.Start();
		break;
		// Maze end
	case 0x04:
		DAMAGEMETER.SetMazeState(TRUE);
		DAMAGEMETER.Suspend();
		break;
	}
}

VOID SWPacketMazeUpdateState::Log() {

}

VOID SWPacketMazeUpdateState::Debug() {
	SWPacketMazeUpdateStatePacket* packet = (SWPacketMazeUpdateStatePacket*)(_data + sizeof(SWHEADER));
	//Log::WriteLogA("[SWPacketMazeUpdateState] stateID = %d, state = %d", packet->_stateID, packet->_state);
}