#pragma once
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)



#pragma pack(pop)

class SWPacketPartyListInfo : public SWPacket {
protected:
	SWPacketPartyListInfo() {}

public:
	SWPacketPartyListInfo(SWHEADER* swheader, BYTE* data);
	~SWPacketPartyListInfo() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};