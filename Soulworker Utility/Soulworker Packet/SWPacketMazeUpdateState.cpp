#include "pch.h"
#include ".\Soulworker Packet\SWPacketMazeUpdateState.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\UI\Option.h"
#include ".\Damage Meter\MapList.h"
#include ".\Packet Capture\PacketCapture.h"

SWPacketMazeUpdateState::SWPacketMazeUpdateState(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMazeUpdateState::Do() {

	SWPacketMazeUpdateStatePacket* packet = (SWPacketMazeUpdateStatePacket*)(_data + sizeof(SWHEADER));
	
	if (!UIOPTION.isSoloRankMode() || rankMap.find(DAMAGEMETER.GetWorldID()) == rankMap.end() || packet->_state != 1)
		return;

	switch (packet->_stateID)
	{
		// Maze waiting(can't use skill)
	case 0x00:
		break;
		// Maze start
	case 0x01:
		if (UIOPTION.isDontSaveUnfinishedMaze())
			DAMAGEMETER.SetTestMode();
		DAMAGEMETER.SetMazeState(FALSE);
		DAMAGEMETER.Start();
		break;
		// Play boss animation
	case 0x02:
		break;
		// End boss animation
	case 0x03:
		break;
		// Maze end
	case 0x04:
		DAMAGEMETER.ClearTestMode();
		DAMAGEMETER.SetMazeState(TRUE);
		DAMAGEMETER.Suspend();
		break;
	}
}

VOID SWPacketMazeUpdateState::Log() {

}

VOID SWPacketMazeUpdateState::Debug() {
	/*SWPacketMazeUpdateStatePacket* packet = (SWPacketMazeUpdateStatePacket*)(_data + sizeof(SWHEADER));
	Log::WriteLogA("[SWPacketMazeUpdateState] stateID = %d, state = %d", packet->_stateID, packet->_state);*/
}