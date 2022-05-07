#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKET_DEAD {
	UINT32 _playerid;
	BYTE _unknown2[7];
}SWPACKET_DEAD;

#pragma pack(pop)

class SWPacketDead : public SWPacket {

protected:
	SWPacketDead() {}

public:
	SWPacketDead(SWHEADER* swheader, BYTE* data);
	~SWPacketDead() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};