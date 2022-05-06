#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKETSTATCHANGE_HEADER {
	BYTE _unknown01;
	BYTE _unknown02; //0430new
	UINT32 _playerID;
	BYTE _statsCount;
}SWPACKETSTATCHANGE_HEADER;

#if defined(SERVER_KOREA)
typedef struct _SWPACKETSTATCHANGE_DATA {
	FLOAT _statValue;
	USHORT _statType;
	BYTE _unknown01[8];
}SWPACKETSTATCHANGE_DATA;
#endif

#if defined(SERVER_STEAM)
typedef struct _SWPACKETSTATCHANGE_DATA {
	FLOAT _statValue;
	USHORT _statType;
	BYTE _unknown01[8]; //0430new
}SWPACKETSTATCHANGE_DATA;
#endif
#if defined(SERVER_JAPAN) // same as KR server
typedef struct _SWPACKETSTATCHANGE_DATA {
	FLOAT _statValue;
	USHORT _statType;
	BYTE _unknown01[8];
}SWPACKETSTATCHANGE_DATA;
#endif

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