#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETBROOCH {
	UINT32 _playerID;
	UINT16 _broochID;
}SWPACKETBROOCH;
#pragma pack(pop)

class SWPacketBrooches : public SWPacket {
protected:
	SWPacketBrooches() {}

public:
	SWPacketBrooches(SWHEADER* swheader, BYTE* data);
	~SWPacketBrooches() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};