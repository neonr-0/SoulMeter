#pragma once
#include ".\Damage Meter\SWDamageMonster.h"
#include ".\Soulworker Packet\PacketInfo.h"
#include <unordered_set>
#include <map>
using namespace std;

#define DEBUG_DAMAGE_PLAYER 0

static unordered_set<UINT32> dpsIgnoreIdList({
	/* 히하(HH) */
	31308106, // 허밋(Hermit)
	31308102, 31308103, 31308104, // 히하 레드/블루/그린 토템 (totem)
	31308105, 31308112, 31308111, 31308114, 31308115, // 신앙심, 신앙심(기믹), 번뇌(80줄), 번뇌(50줄), 번뇌(20줄)
	31308110, 31308113, 31308116, // 분신(돌진), 분신(회전), 분신(랜덤돌진) 

	/* 루나폴(LF) */
	31309109, 31309154, // 저지먼트 크러셔, 추적토템

	/* 프멀, 히프멀(Primal, Hero Primal) */
	// 나중에...

	/* 브세(BS) */
	31310111, 31310112, // 기믹용 추정
	31310116, 31310117, // 테네브리스의 불꽃 (빛,어둠)
	31310151, 31310152, 31310153, 31310154, // 붐 퍼펫, 포스 피스트, 쫄몹34? (남은건 헤비 워리어, 데들리 나이프인데 누가 누군지는 아직 모름)
	31310192, // 2페 상자

	//31310108, 31310109, // 아마 얼음이랑 봉자
	31310118, 31310119, // 이게 진짜 얼음이랑 봉자 (위에껀 오타)

	/* 기타(etc) */
	14000101, 14000102, 14000103, // 퍼펫 빨강/파랑/금색 구슬 (puppet orb)
	1081102, 1081103, 1081104, 1081105, 1081106, 1081107, 1081108, 1081109, // 옥타곤 1렙~5렙+특성3렙 (octagon)
	1091101 // 폭심지 (wicked bomb)

	});

class SWDamagePlayer : public MemoryPool<SWDamagePlayer, 30> {
private:
	UINT32 _id;
	UINT64 _damage;
	UINT64 _soulstoneDamage;
	USHORT _hitCount;
	USHORT _critHitCount;
	USHORT _maxCombo;
	USHORT _hitCountForCritRate;
	USHORT _critHitCountForCritRate;
	USHORT _missCount;
	USHORT _soulstoneCount;
	UINT64 _damageForSoulstone;
	UINT64 _soulstoneDamageForSoulstone;
	USHORT _hitCountForArmorBreak;
	FLOAT _armorBreak;
	FLOAT _sg;
	FLOAT _stamina;
	FLOAT _sv;
	FLOAT _attackSpeed;
	FLOAT _partialDamage;
	USHORT _getHitAll;
	USHORT _getHit;
	USHORT _getHitBS;
	USHORT _getHitMissed;
	USHORT _getHitMissedReal;
	FLOAT _enlightenSum;
	USHORT _gigaEnlightenProc;
	USHORT _teraEnlightenProc;
	USHORT _skillCounts = 0;
	USHORT _dodgeCounts = 0;
	USHORT _deathCounts = 0;

	DOUBLE _historyAvgAB = 0.0;

	DOUBLE _historygear90 = 0.0;
	DOUBLE _historygear50 = 0.0;
	DOUBLE _historyacc01 = 0.0;
	DOUBLE _historyacc02 = 0.0;

	DOUBLE _historyLosedHP = 0.0;
	BYTE _JqStack = 0;

	vector<SWDamageMonster*> _monsterInfo;
	VOID InsertMonsterInfo(UINT32 monsterID, UINT64 damage, UINT64 critDamage, USHORT hitCount, USHORT critHitCount, UINT32 skillID);
	VOID Sort();

protected:
	SWDamagePlayer() : _id(0), _damage(0), _soulstoneDamage(0), _hitCount(0), _critHitCount(0), _maxCombo(0), 
		_hitCountForCritRate(0), _critHitCountForCritRate(0), _soulstoneCount(0), _hitCountForArmorBreak(0),
		_armorBreak(0), _sg(0), _stamina(0), _sv(0), _attackSpeed(0), _partialDamage(0.0f) { }

public:
	SWDamagePlayer(UINT32 id, UINT64 damage, UINT64 soulstoneDamage, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID);
	SWDamagePlayer(UINT32 id);
	~SWDamagePlayer();

	map<UINT32, USHORT> skillCounts;

	static BOOL SortFunction(SWDamagePlayer* playerA, SWDamagePlayer* playerB);

	UINT32 GetID();
	UINT64 GetDamage();
	UINT64 GetSoulstoneDamage();
	USHORT GetHitCount();
	USHORT GetCritHitCount();
	USHORT GetMaxCombo();
	UINT64 GetMonsterTotalDamage();
	USHORT GetHitCountForCritRate();
	USHORT GetCritHitCountForCritRate();
	USHORT GetMissCount();
	USHORT GetSoulstoneCount();
	UINT64 GetDamageForSoulstone();
	UINT64 GetSoulStoneDamageForSoulstone();
	USHORT GetGetHitAll();
	USHORT GetGetHit();
	USHORT GetGetHitBS();
	USHORT GetGetHitMissed();
	USHORT GetGetHitMissedReal();
	FLOAT GetEnlightenSum();
	USHORT GetGigaEnlighten();
	USHORT GetTeraEnlighten();
	USHORT GetSkillUsed();
	USHORT GetDodgeUsed();
	USHORT GetDeathCount();

	VOID SetHistoryAvgAB(DOUBLE historyAvgAB);
	DOUBLE GetHistoryAvgAB();

	VOID setHistoryBS(int type, DOUBLE value);
	DOUBLE GetHistoryBS(int type);

	VOID setHistoryLosedHP(DOUBLE losedHP);
	DOUBLE GetHistoryLosedHP();

	VOID SetJqStack(BYTE stack);
	BYTE GetJqStack();

	VOID AddGetDamage(UINT64 totalDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, UINT32 monsterID, UINT32 skillID);
	VOID AddEnlighten(FLOAT value);
	VOID AddSkillUsed(UINT32 skillId);
	VOID AddDodgeUsed();
	VOID AddDeathCount();

	vector<SWDamageMonster*>::const_iterator GetMonsterInfo(UINT id);
	vector<SWDamageMonster*>::const_iterator begin();
	vector<SWDamageMonster*>::const_iterator end();
	const SIZE_T& size();

	VOID AddDamage(UINT64 totalDMG, UINT64 soulstoneDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID);
};