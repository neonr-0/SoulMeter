#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETGESTUREUSED {
	UINT32 _playerID;
	UINT32 _gestureId;
	FLOAT _locationX;
	FLOAT _locationY;
	FLOAT _locationZ;
	FLOAT _direction;
	FLOAT _screen;
}SWPACKETGESTUREUSED;
#pragma pack(pop)

class SWPacketGestureUsed : public SWPacket {

protected:
	SWPacketGestureUsed() {}

public:
	SWPacketGestureUsed(SWHEADER* swheader, BYTE* data);
	~SWPacketGestureUsed() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};