#pragma once

#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#define SWPACKETSQUAD_DUMMY 64

#pragma pack(push, 1)

typedef struct _SWPACKETSQUAD_HEADER_P1 {
	BYTE _unknown01[12];
	USHORT _squadNameSize;
	// variable header 
	// _squadName
	// _squadLeadNameSize
	// _squadLeadName
}SWPACKETSQUAD_HEADER_P1; 

typedef struct _SWPACKETSQUAD_HEADER_P2 {
	BYTE _unknown01[4];
	BYTE _squadMaxPlayerCount; //??
	BYTE _unknown02[7];
	BYTE _squadCurPlayerCount;
	BYTE _unknown03[3];
}SWPACKETSQUAD_HEADER_P2;

typedef struct _SWPACKETSQUAD_DATA {
	UINT32 _playerID;
	USHORT _nickSize;
}SWPACKETSQUAD_DATA;

#pragma pack(pop)

class SWPacketSquad : public SWPacket {
protected:
	SWPacketSquad() {}

public:
	SWPacketSquad(SWHEADER* swheader, BYTE* data);
	~SWPacketSquad() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};