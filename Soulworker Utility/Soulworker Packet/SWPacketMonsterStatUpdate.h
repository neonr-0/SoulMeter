#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPacketMonsterStatUpdate {
	UINT32 _id;
	BYTE _statCounts;
}SWPacketMonsterStatUpdatePkt;

typedef struct _SWPacketMonsterStatData {
	FLOAT _unk01;
	USHORT _statID;
	UINT64 _statVal;
}SWPacketMonsterStatData;
#pragma pack(pop)

class SWPacketMonsterStatUpdate : public SWPacket {

protected:
	SWPacketMonsterStatUpdate() {}

public:
	SWPacketMonsterStatUpdate(SWHEADER* swheader, BYTE* data);
	~SWPacketMonsterStatUpdate() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};