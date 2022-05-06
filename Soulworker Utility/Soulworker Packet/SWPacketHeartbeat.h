#pragma once
#include ".\Soulworker Packet\SWPacket.h"

/*
* 도통 뭔지 모르겠음 ㅅㅂ
* 1초마다 계속 날아옴
*/

#pragma pack(push, 1)
typedef struct _SWPACKETHEARTBEAT {
	UINT32 _millisecond; // 1000이 1초인 무언가
	BYTE _unknown01[4];
}SWPACKETHEARTBEAT;

#pragma pack(pop)

class SWPacketHeartbeat : public SWPacket {
protected:
	SWPacketHeartbeat() {}

public:
	SWPacketHeartbeat(SWHEADER* swheader, BYTE* data);
	~SWPacketHeartbeat() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};