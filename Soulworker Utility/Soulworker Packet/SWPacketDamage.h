#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Damage Meter\MonsterList.h"

#pragma pack(push, 1)

typedef struct _SWPACKETDAMAGE_MONSTER {
	UINT32 _monsterID; // 4
	BYTE _unknown01; // 1
	BYTE _damageType; //1
	UINT32 _totalDMG; //4
	UINT32 _soulstoneDMG; // 4
	UINT32 _remainHP; //4
	FLOAT _unknownF1; // 4
	FLOAT _unknownF2; // 4
	FLOAT _unknownF3; // 4
	BYTE _unknown02[22]; // 0605
}SWPACKETDAMAGE_MONSTER;

typedef struct _SWPACKETDAMAGE_PLAYER {
	UINT32 _playerID;
	BYTE _unknown01[20]; //0403new
	UINT32 _skillID; 
	BYTE _unknown02[2]; //0403new
	USHORT _maxCombo;
	BYTE _unknown03[2]; //0403new
}SWPACKETDAMAGE_PLAYER;
	
#pragma pack(pop)

class SWPacketDamage : public SWPacket {
private:
	VOID AttackOfMonster();

protected:
	SWPacketDamage() {}

public:
	SWPacketDamage(SWHEADER* swheader, BYTE* data);
	~SWPacketDamage() {}

	VOID Do();
	VOID Debug();
	VOID Log() {};
};