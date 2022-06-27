#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETHEARTBEAT {
	BYTE _unknown01; //0430new
	UINT32 _tick; // tick
	BYTE _unknown02[4];
}SWPACKETHEARTBEAT;

#pragma pack(pop)

class SWPacketHeartbeat : public SWPacket {
protected:
	SWPacketHeartbeat() {}

	INT64 _ts;

public:
	SWPacketHeartbeat(SWHEADER* swheader, BYTE* data, UINT64 ts);
	~SWPacketHeartbeat() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};