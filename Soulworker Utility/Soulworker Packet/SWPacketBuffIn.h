#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETBUFFIN {
	UINT32 _playerID;
	USHORT _buffID;
	FLOAT _duration;
	BYTE _stack;
	UINT32 _giverID;
	BYTE _unknown02;
}SWPACKETBUFFIN;
#pragma pack(pop)

class SWPacketBuffIn : public SWPacket {
protected:
	SWPacketBuffIn() {}

public:
	SWPacketBuffIn(SWHEADER* swheader, BYTE* data);
	~SWPacketBuffIn() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};