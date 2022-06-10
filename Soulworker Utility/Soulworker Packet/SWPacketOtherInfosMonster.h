#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKET_OTHER_INFOS_MONSTER1 {

	UINT32 id;

	UINT32 unk01;
	UINT32 unk02;
	UINT32 unk03;

	UINT32 unk04;
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
	ULONG64 unk14;
	ULONG64 unk15;
	BYTE data1Count;

}SWPACKET_OTHER_INFOS_MONSTER1;

typedef struct _SWPACKET_OTHER_INFOS_MONSTER_DATA1 {

	BYTE type;
	FLOAT val;

}SWPACKET_OTHER_INFOS_MONSTER_DATA1;

typedef struct _SWPACKET_OTHER_INFOS_MONSTER2 {

	ULONG64 unk01;
	ULONG64 unk02;
	UINT32 unk03;
	BYTE data2Count;

}SWPACKET_OTHER_INFOS_MONSTER2;

typedef struct _SWPACKET_OTHER_INFOS_MONSTER_DATA2 {
	
	UINT32 unk01;
	UINT32 unk02;
	BYTE unk03;
	UINT32 unk04;
	BYTE unk05;

}SWPACKET_OTHER_INFOS_MONSTER_DATA2;

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