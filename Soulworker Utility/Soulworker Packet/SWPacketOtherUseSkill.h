#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"

#pragma pack(push, 1)

typedef struct _SWPACKET_OTHERUSESKILL {
	UINT32 _playerId;
	UINT32 _skillId;
	FLOAT _locationX;
	FLOAT _locationY;
	FLOAT _locationZ;
	FLOAT _direction;
	//BYTE _unknown01[19]; // 0430new
}SWPACKET_OTHERUSESKILL;

#pragma pack(pop)

class SWPacketOtherUseSkill : public SWPacket {

private:
	CHAR _skillName[SKILL_NAME_LEN];

protected:
	SWPacketOtherUseSkill() {}

public:
	SWPacketOtherUseSkill(SWHEADER* swheader, BYTE* data);
	~SWPacketOtherUseSkill() {}

	VOID Do();
	VOID Debug();
	VOID Log();
};