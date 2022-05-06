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
	BYTE _unknown02[18]; // //0430new
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
	31309102, // 루나폴 - 에드가
	31301001, // 바썬 - 플레마 1페
	31301002, // 바썬 - 플레마 2페
	31310101 // 브세노말 - 테네브리스 1페
	// 702 // 일시정지 테스트용 트센몹
	});
static unordered_set<UINT32> endIdList({
	31308001, // 히하 - 라파쿰바
	31309151, // 루나폴 - 정크 퀸
	31301003, // 바썬 - 플레마 3페
	31310102 // 브세노말 - 테네브리스 2페
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