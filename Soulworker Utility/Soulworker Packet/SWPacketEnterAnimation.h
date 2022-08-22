#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKETENTERANIMATION {
	UINT32 _player_id;
	UINT32 _type1;
	UINT32 _type2;
}SWPACKETENTERANIMATION;

#pragma pack(pop)

class SWPacketEnterAnimation : public SWPacket {
protected:
	SWPacketEnterAnimation() {}

public:
	SWPacketEnterAnimation(SWHEADER* swheader, BYTE* data);
	~SWPacketEnterAnimation() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};