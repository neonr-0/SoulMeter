#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPACKETBUFFOUT {
	UINT32 _playerID;
	USHORT _buffID;
	BYTE _unknwon01;
	UINT32 _giverID;
}SWPACKETBUFFOUT;
#pragma pack(pop)

class SWPacketBuffOut : public SWPacket {
protected:
	SWPacketBuffOut() {}

public:
	SWPacketBuffOut(SWHEADER* swheader, BYTE* data);
	~SWPacketBuffOut() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};