#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)



#pragma pack(pop)

class SWPacketOtherInfosMonster : public SWPacket {
protected:
	SWPacketOtherInfosMonster() {}

public:
	SWPacketOtherInfosMonster(SWHEADER* swheader, BYTE* data);
	~SWPacketOtherInfosMonster() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};