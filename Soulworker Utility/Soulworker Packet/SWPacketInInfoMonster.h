#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKET_IN_INFO_MONSTER1 {

	UINT32 id;

	FLOAT unk01;
	FLOAT unk02;
	FLOAT unk03;

	FLOAT unk04;
	ULONG64 unk05;
	ULONG64 unk06;
	UINT32 unk07;
	UINT32 unk08;
	BYTE unk09;
	UINT32 realDB2;

	UINT32 owner_id;
	UINT32 unk11;
	UINT32 unk12;
	BYTE unk13;
	DOUBLE unk14;
	DOUBLE unk15;
	BYTE data1Count;

}SWPACKET_IN_INFO_MONSTER1;

typedef struct _SWPACKET_IN_INFO_MONSTER_DATA1 {

	BYTE type;
	FLOAT val;

}SWPACKET_IN_INFO_MONSTER_DATA1;

typedef struct _SWPACKET_IN_INFO_MONSTER2 {

	ULONG64 unk01;
	ULONG64 unk02;
	FLOAT unk03;
	BYTE data2Count;

}SWPACKET_IN_INFO_MONSTER2;

typedef struct _SWPACKET_IN_INFO_MONSTER_DATA2 {

	UINT32 unk01;
	FLOAT unk02;
	BYTE unk03;
	UINT32 unk04;
	BYTE unk05;

}SWPACKET_IN_INFO_MONSTER_DATA2;

#pragma pack(pop)

class SWPacketInInfoMonster : public SWPacket {
protected:
	SWPacketInInfoMonster() {}

public:
	SWPacketInInfoMonster(SWHEADER* swheader, BYTE* data);
	~SWPacketInInfoMonster() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};