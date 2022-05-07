#pragma once
#include ".\Language\Region.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#if defined(SERVER_KOREA)
#define SWPACKETPOS_DUMMY 36
#endif
#if defined(SERVER_STEAM)
#define SWPACKETPOS_DUMMY 32
#endif
#if defined(SERVER_JAPAN)
#define SWPACKETPOS_DUMMY 36 // same as KR server? (not tested yet)
#endif

#pragma pack(push, 1)

typedef struct _SWPACKETPOS_HEADER {
	BYTE _unknown01[4];
	UINT32 _posHostID;
	BYTE _unknown02[10];
	BYTE _posPlayerCount;
}SWPACKETPOS_HEADER;

typedef struct _SWPACKETPOS_DATA {
	UINT32 _playerID;
	USHORT _nickSize;
}SWPACKETPOS_DATA;

#pragma pack(pop)

class SWPacketPos : public SWPacket {
protected:
	SWPacketPos() {}

public:
	SWPacketPos(SWHEADER* swheader, BYTE* data);
	~SWPacketPos() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};