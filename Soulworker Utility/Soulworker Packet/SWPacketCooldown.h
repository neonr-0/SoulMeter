#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETCOOLDOWN {
	BYTE _unknown01; //0430new
	UINT32 _playerID;
	FLOAT _value;
}SWPACKETCOOLDOWN;
#pragma pack(pop)

class SWPacketCooldown : public SWPacket {
protected:
	SWPacketCooldown() {}

public:
	SWPacketCooldown(SWHEADER* swheader, BYTE* data);
	~SWPacketCooldown() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};