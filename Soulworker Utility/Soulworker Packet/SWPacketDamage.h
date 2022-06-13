#pragma once
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include <unordered_set>

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
#ifdef SERVER_KOREA
	BYTE _unknown02[22]; // sum  // previous value 18
#endif
#ifdef SERVER_STEAM
	BYTE _unknown02[22]; // 0605
#endif
#ifdef SERVER_JAPAN
    BYTE _unknown02[18]; // same as past of KR server
#endif
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
static unordered_set<UINT32> pauseIdList({
	31309102, // 
	31301001, // 
	31301002, // 
	31310101, // 
	// 702 // 
	31310103 // BS Hard Phase 1
	});
static unordered_set<UINT32> endIdList({
	31308001, // 
	31309151, // 
	31301003, // 
	31310102, // 
	31310107, // BS Hard Phase 2
	32308101, //raphakumba HH
	32301001, //flemma solo
	});

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