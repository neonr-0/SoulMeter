#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKETSTARTMOVE {
	UINT32 _playerId;
	BYTE _unknown01[4];
	USHORT _mapId;
	BYTE _unknown02[2];
	FLOAT _locationX;
	FLOAT _locationY;
	FLOAT _locationZ;
	FLOAT _direction;
	FLOAT _endLocationX;
	FLOAT _endLocationY;
	BYTE _screen[4];
	BYTE _unknown04;
	FLOAT _speed;
	BYTE _unknown05[6];
}SWPACKETSTARTMOVE;

#pragma pack(pop)

class SWPacketStartMove : public SWPacket {
protected:
	SWPacketStartMove() {}

public:
	SWPacketStartMove(SWHEADER* swheader, BYTE* data);
	~SWPacketStartMove() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};