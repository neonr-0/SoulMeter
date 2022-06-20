#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKETSTATCHANGE_HEADER {
	BYTE _unknown01;
	UINT32 _playerID;
	BYTE _statsCount;
}SWPACKETSTATCHANGE_HEADER;

typedef struct _SWPACKETSTATCHANGE_DATA {
	FLOAT _statValue;
	USHORT _statType;
	BYTE _unknown01[8]; //0430new
}SWPACKETSTATCHANGE_DATA;

#pragma pack(pop)

class SWPacketStatChange : public SWPacket {

protected:
	SWPacketStatChange() {}

public:
	SWPacketStatChange(SWHEADER* swheader, BYTE* data);
	~SWPacketStatChange() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};