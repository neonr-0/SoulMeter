#pragma once

#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWSPacket.h"

#pragma pack(push, 1)

typedef struct _SWSPACKET_HEARTBEAT {
	BYTE unk01;
	UINT32 _ms;
}_SWSPACKET_HEARTBEAT;

#pragma pack(pop)


class SWSPacketHeartbeat : public SWSPacket {
protected:
	SWSHEADER* _swheader;
	BYTE* _data;

	SWSPacketHeartbeat() {}
	SWSPacketHeartbeat(SWSPacket& other) {}

public:
	SWSPacketHeartbeat(SWSHEADER* swheader, BYTE* data);
	virtual ~SWSPacketHeartbeat() {}

	virtual VOID Debug();
	virtual VOID Log();
	virtual VOID Do();
};