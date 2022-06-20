#pragma once

#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWSPacket.h"
#include <unordered_set>

#pragma pack(push, 1)

typedef struct _SWPACKET_MYDODGEUSED {
	UINT32 _skillId;
	UINT32 _playerId;
	BYTE _unknown01[46];
}SWPACKET_MYDODGEUSED;

#pragma pack(pop)

static unordered_set<UINT32> normalDodgeList({
	12040011, 12040012, 12040013,
	22040011, 22040012, 22040013,
	32040011, 32040012, 32040013,
	42040011, 42040012, 42040013,
	52040011, 52040012, 52040013,
	62040011, 62040012, 62040013,
	72040011, 72040012, 72040013,
	82040011, 82040012, 82040013,
	92040011, 92040012, 92040013,
	102040011, 102040012, 102040013
	});


class SWSPacketMyDodgeUsed : public SWSPacket {
protected:
	SWSHEADER* _swheader;
	BYTE* _data;

	SWSPacketMyDodgeUsed() {}
	SWSPacketMyDodgeUsed(SWSPacket& other) {}

public:
	SWSPacketMyDodgeUsed(SWSHEADER* swheader, BYTE* data);
	virtual ~SWSPacketMyDodgeUsed() {}

	virtual VOID Debug();
	virtual VOID Log();
	virtual VOID Do();
};