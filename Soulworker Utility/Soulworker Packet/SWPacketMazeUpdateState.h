#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)

typedef struct _SWPacketMazeUpdateStatePacket {
	
	UINT32 _stateID;
	BYTE _state;

}SWPacketMazeUpdateStatePacket;
#pragma pack(pop)

class SWPacketMazeUpdateState : public SWPacket {
protected:
	SWPacketMazeUpdateState() {}

public:
	SWPacketMazeUpdateState(SWHEADER* swheader, BYTE* data);
	~SWPacketMazeUpdateState() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};