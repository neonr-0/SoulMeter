#pragma once

#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#define SWPACKETBIGPARTY_DUMMY 38 //1117

#pragma pack(push, 1)

typedef struct _SWPACKETBIGPARTY_HEADER {
	BYTE _unknown01[4];
	UINT32 _posHostID;
	BYTE _unknown02[10];
	BYTE _posPlayerCount;
}SWPACKETBIGPARTY_HEADER;

typedef struct _SWPACKETBIGPARTY_DATA {
	UINT32 _playerID;
	USHORT _nickSize;
}SWPACKETBIGPARTY_DATA;

#pragma pack(pop)

class SWPacketBigParty : public SWPacket {
protected:
	SWPacketBigParty() {}

public:
	SWPacketBigParty(SWHEADER* swheader, BYTE* data);
	~SWPacketBigParty() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};