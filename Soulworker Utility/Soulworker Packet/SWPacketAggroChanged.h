#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETAGGROCHANGED {
	UINT32 _id;
	UINT32 _targetedId;
}SWPACKETAGGROCHANGED;
#pragma pack(pop)

class SWPacketAggroChanged : public SWPacket {

protected:
	SWPacketAggroChanged() {}

public:
	SWPacketAggroChanged(SWHEADER* swheader, BYTE* data);
	~SWPacketAggroChanged() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};