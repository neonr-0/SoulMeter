#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETMONSTERKILLED {
	UINT32 _killedId;
	//BYTE _unknown01[4];//3
}SWPACKETMONSTERKILLED;
#pragma pack(pop)

class SWPacketMonsterKilled : public SWPacket {

protected:
	SWPacketMonsterKilled() {}

public:
	SWPacketMonsterKilled(SWHEADER* swheader, BYTE* data);
	~SWPacketMonsterKilled() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};