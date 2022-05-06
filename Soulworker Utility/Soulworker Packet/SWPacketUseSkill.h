#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

//typedef struct _SWPACKETCHAT_HEADER {
//	UINT32 _playerID;
//	BYTE _chatType;
//	BYTE _unknown01;
//	USHORT _unknown02;
//	USHORT _chatSize;
//}SWPACKETCHAT_HEADER;
#pragma pack(pop)

class SWPacketUseSkill : public SWPacket {

protected:
	SWPacketUseSkill() {}

public:
	SWPacketUseSkill(SWHEADER* swheader, BYTE* data);
	~SWPacketUseSkill() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};