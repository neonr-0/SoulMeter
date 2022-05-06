#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKETAKASIC {
	BYTE unk01; // 0403new
	UINT32 _ownder_id; // 0403new??
	UINT32 _id;
}SWPACKETAKASIC;

#pragma pack(pop)

class SWPacketAkasic : public SWPacket {
protected:
	SWPacketAkasic() {}

public:
	SWPacketAkasic(SWHEADER* swheader, BYTE* data);
	~SWPacketAkasic() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};