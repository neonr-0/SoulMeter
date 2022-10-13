#pragma once
#include "pch.h"

#include ".\Damage Meter\Timer.h"
#include ".\Damage Meter\SWDamagePlayer.h"
#include ".\UI\PlotWindow.h"
#include ".\Damage Meter\MonsterList.h"
#include ".\UI\Option.h"
#include <unordered_map>
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define MAX_NAME_LEN 64
//#define MAX_MAP_LEN 32
#define MAX_MAP_LEN 64

#define PLAYER_JOB_CANT_FIND 10

#ifdef _DEBUG
#define DEBUG_DAMAGEMETER_OWNER_ID 0
#define DEBUG_DAMAGEMETER_DB 0
#define DEBUG_DAMAGEMETER_WORLD 0
#define DEBUG_DAMAGEMETER_MYID 0
#define DEBUG_DAMAGEMETER_PLAYERMETA 0
#endif

// 
#define DAMAGEMETER SWDamageMeter::getInstance()

//  forward declaration
// https://stackoverflow.com/questions/41502310/c-inclusion-of-typedef-struct-from-inside-a-class
// https://en.cppreference.com/w/cpp/language/class#Forward_declaration

typedef struct _SW_OWNER_ID_STRUCT {
	UINT32 _id;
	UINT32 _owner_id;
}SW_OWNER_ID_STRUCT;

typedef struct _SW_DB2_STRUCT {
	UINT32 _id;
	UINT32 _db2;
	INT32 _type;
}SW_DB2_STRUCT;

static vector<UINT32> _dwSkills({

	// Haru
	15000111,
	15000211,
	16000111, // Frenzy
	15130111, // ??
	17000111, // EX

	// Erwin
	25000111,
	25000211,
	26000111,
	25130111, // ??
	27000111,

	// Lily
	35000111,
	35000211,
	36000111,
	35130111, // ??
	37000111,

	// Jin
	45000111,
	45000211,
	46000111,
	45130111, // ??
	47000111,

	// Stella
	55000111,
	55000211,
	56000111,
	55130111, // ??
	57000111,

	// Iris
	65000111,
	65000211,
	66000111,
	67000111,

	// Chii
	75000111,
	75000211,
	76000111,
	77000111,

	// Ephnel
	85000111,
	85000211,
	86000111,
	87000111,

	// Nabi
	95000111,
	95000211,
	96000111,
	97000111,

	});

class SWDamageMeter : public Singleton<SWDamageMeter> {
public: typedef struct _SW_PLAYER_METADATA {
private:
	FLOAT _armorBreak = 0.0;
	FLOAT _sg = 0.0;
	FLOAT _stamina = 0.0;
	FLOAT _sv = 0.0;
	FLOAT _attackSpeed = 0.0;
	FLOAT _partialDamage = 0.0;
	FLOAT _maxHP = 0.0;
	FLOAT _currentHP = 0.0;
	FLOAT _maxAttack = 0.0;
	FLOAT _critDamage = 0.0;
	FLOAT _bossDamage = 0.0;
public:
	UINT32 _id = 0;
	CHAR _name[MAX_NAME_LEN];
	BYTE _job = 0;
	UINT64 _avgABSum = 0;
	UINT64 _avgABPreviousTime = 0;

	UINT64 _avgBDSum = 0;
	UINT64 _avgBDPreviousTime = 0;

	UINT64 _avgASSum = 0;
	UINT64 _avgASPreviousTime = 0;

	BOOL _gear90EffectStarted = false;
	UINT64 _gear90Sum = 0;
	UINT64 _gear90EffectStartedTime = 0;

	BOOL _gear50EffectStarted = false;
	UINT64 _gear50Sum = 0;
	UINT64 _gear50EffectStartedTime = 0;

	BOOL _acc01EffectStarted = false;
	UINT64 _acc01Sum = 0;
	UINT64 _acc01EffectStartedTime = 0;

	BOOL _acc02EffectStarted = false;
	UINT64 _acc02Sum = 0;
	UINT64 _acc02EffectStartedTime = 0;

	DOUBLE _losedHp = 0;

	BOOL _fullABStarted = false;
	UINT64 _fullABStartTime = 0;
	UINT64 _fullABEndTime = 0;
	DOUBLE _fullABPrevTime = 0;
	DOUBLE _fullABTime = 0;

	BOOL _AggroStarted = false;
	UINT64 _AggroStartTime = 0;
	UINT64 _AggroEndTime = 0;
	DOUBLE _AggroPrevTime = 0;
	DOUBLE _AggroTime = 0;

	BOOL _fullASStarted = false;
	UINT64 _fullASStartTime = 0;
	UINT64 _fullASEndTime = 0;
	DOUBLE _fullASPrevTime = 0;
	DOUBLE _fullASTime = 0;


	_SW_PLAYER_METADATA() {
		_id = 0;
		_job = 0;
		_avgABSum = 0;
		_avgABPreviousTime = 0;
		_avgBDSum = 0;
		_avgBDPreviousTime = 0;
	}

	VOID UpdateStat(USHORT statType, FLOAT statValue) {
		switch (statType) {
		case StatType::SG:
			_sg = statValue;
			break;
		case StatType::Stamina:
			_stamina = statValue;
			break;
		case StatType::SV:
			_sv = statValue;
			break;
		case StatType::AttackSpeed:
			_attackSpeed = statValue;
			if (DAMAGEMETER.isRun()) {
				UINT64 time = (UINT64)((DOUBLE)DAMAGEMETER.GetTime()); // timer time
				FLOAT correctedAS = (_attackSpeed > 250) ? 250 : _attackSpeed;
				_avgASSum += static_cast<UINT64>((time - _avgASPreviousTime) * correctedAS);
				_avgASPreviousTime = time;

				if (_attackSpeed >= 250) {
					if (!_fullASStarted) {
						_fullASStarted = true;
						_fullASStartTime = time;
						_fullASPrevTime = _fullASTime;
					}
					_fullASEndTime = time;
				}
				else if (_fullASStarted) {
					_fullASStarted = false;
					_fullASEndTime = time;
				}
				CalcFullASTime();
			}
			break;
		case StatType::PartialDamage:
			_partialDamage = statValue;
			break;
		case StatType::ArmorBreak:
			_armorBreak = statValue;
			if (DAMAGEMETER.isRun()) {
				UINT64 time = (UINT64)((DOUBLE)DAMAGEMETER.GetTime()); // timer time
				FLOAT correctedAB = (_armorBreak > 100) ? 100 : _armorBreak;
				_avgABSum += static_cast<UINT64>((time - _avgABPreviousTime) * correctedAB);
				_avgABPreviousTime = time;

				if (_armorBreak >= 100) {
					if (!_fullABStarted) {
						_fullABStarted = true;
						_fullABStartTime = time;
						_fullABPrevTime = _fullABTime;
					}
					_fullABEndTime = time;
				}
				else if (_fullABStarted) {
					_fullABStarted = false;
					_fullABEndTime = time;
				}
				CalcFullABTime();
			}
			break;
		case StatType::CurrentHP:
			if (_currentHP > 0.0 && _currentHP > statValue) {
				_losedHp += ((DOUBLE)_currentHP - (DOUBLE)statValue);
			}
			_currentHP = statValue;
			break;
		case StatType::MaxHP:
			_maxHP = statValue;
			break;
		case StatType::MinAttack:
			_maxAttack = static_cast<FLOAT>(statValue * 1.25);
			break;
		case StatType::MaxAttack:
			_maxAttack = statValue;
			break;
		case StatType::CritDamage:
			_critDamage = statValue;
			break;

		default:
			//Log::WriteLog(_T("[DEBUG] [statType = %x], [statValue = %f]\n"), statType, statValue);
			break;
		}
	}

	VOID UpdateSpecialStat(USHORT statType, FLOAT statValue) {
		switch (statType) {
		case SpecialStatType::BossDamageAddRate:
			_bossDamage = statValue;
			if (DAMAGEMETER.isRun()) {
				UINT64 time = (UINT64)((DOUBLE)DAMAGEMETER.GetTime());
				_avgBDSum += static_cast<UINT64>((time - _avgBDPreviousTime) * _bossDamage);
				_avgBDPreviousTime = time;
			}
			break;
		default:
			//Log::WriteLog(_T("[DEBUG] [statType = %x], [statValue = %f]\n"), statType, statValue);
			break;
		}
	}

	VOID UpdateAggroTime(BOOL isSet)
	{
		UINT64 time = (UINT64)((DOUBLE)DAMAGEMETER.GetTime()); // timer time

		if (isSet) {
			if (!_AggroStarted) {
				_AggroStarted = true;
				_AggroStartTime = time;
				_AggroPrevTime = _AggroTime;
			}
			_AggroEndTime = time;
		}
		else if (_AggroStarted) {
			_AggroStarted = false;
			_AggroEndTime = time;
		}
		CalcAggroTime();
	}

	VOID MeterSuspended() {
		UINT64 currentTime = (UINT64)DAMAGEMETER.GetTime();

		FLOAT correctedAB = (_armorBreak > 100) ? 100 : _armorBreak;
		_avgABSum += static_cast<UINT64>((currentTime - _avgABPreviousTime) * correctedAB);
		_avgABPreviousTime = currentTime;

		_avgBDSum += static_cast<UINT64>((currentTime - _avgBDPreviousTime) * _bossDamage);
		_avgBDPreviousTime = currentTime;

		FLOAT correctedAS = (_attackSpeed > 250) ? 250 : _attackSpeed;
		_avgASSum += static_cast<UINT64>((currentTime - _avgASPreviousTime) * correctedAS);
		_avgASPreviousTime = currentTime;

		if (_fullABStarted) {
			_fullABStarted = false;
			_fullABEndTime = currentTime;
		}
		CalcFullABTime();

		if (_AggroStarted) {
			_AggroStarted = false;
			_AggroEndTime = currentTime;
		}
		CalcAggroTime();

		if (_fullASStarted) {
			_fullASStarted = false;
			_fullASEndTime = currentTime;
		}
		CalcFullASTime();
	}

	VOID MeterReseted() {
		auto player = DAMAGEMETER.GetPlayerInfo(_id);
		if (player != DAMAGEMETER.end()) {
			UINT64 currentTime = (UINT64)DAMAGEMETER.GetTime();

			(*player)->setHistoryLosedHP(_losedHp);

			if (_AggroStarted) {
				_AggroStarted = false;
				_AggroEndTime = currentTime;
				CalcAggroTime();
			}
			(*player)->SetHistoryAggroTime(_AggroTime);

			if (_id == DAMAGEMETER.GetMyID()) {

				UINT64 avgTimeDifference = currentTime - _avgABPreviousTime;
				DOUBLE currentAB = GetStat(StatType::ArmorBreak);
				currentAB = currentAB > 100.0 ? 100.0 : currentAB; // 
				UINT64 calculatedAvgAB = static_cast<UINT64>((_avgABSum + avgTimeDifference * currentAB));
				(*player)->SetHistoryAvgAB((DOUBLE)calculatedAvgAB / currentTime);

				avgTimeDifference = currentTime - _avgBDPreviousTime;
				UINT64 calculatedAvgBD = static_cast<UINT64>((_avgBDSum + avgTimeDifference * _bossDamage));
				(*player)->SetHistoryAvgBD((DOUBLE)calculatedAvgBD / currentTime);

				DOUBLE currentAS = GetStat(StatType::AttackSpeed);
				currentAS = currentAS > 250 ? 250.0 : currentAS;
				avgTimeDifference = currentTime - _avgASPreviousTime;
				UINT64 calculatedAvgAS = static_cast<UINT64>((_avgASSum + avgTimeDifference * currentAS));
				(*player)->SetHistoryAvgAS((DOUBLE)calculatedAvgAS / currentTime);

				if (_fullABStarted) {
					_fullABStarted = false;
					_fullABEndTime = currentTime;
					CalcFullABTime();
				}
				(*player)->SetHistoryABTime(_fullABTime);

				if (_fullASStarted) {
					_fullASStarted = false;
					_fullASEndTime = currentTime;
					CalcFullASTime();
				}
				(*player)->SetHistoryASTime(_fullASTime);

				// 
				if (_gear90EffectStarted) {
					UINT64 gear90TimeDiff = currentTime - _gear90EffectStartedTime;
					gear90TimeDiff = (gear90TimeDiff >= 5000) ? 5000 : gear90TimeDiff;
					_gear90Sum += gear90TimeDiff * 500;
					_gear90EffectStarted = false;
				}

				// 
				if (_gear50EffectStarted) {
					UINT64 gear50TimeDiff = currentTime - _gear50EffectStartedTime;
					gear50TimeDiff = (gear50TimeDiff >= 5000) ? 5000 : gear50TimeDiff;
					_gear50Sum += gear50TimeDiff * 1000;
					_gear50EffectStarted = false;
				}

				//
				if (_acc01EffectStarted) {
					UINT64 acc01TimeDiff = currentTime - _acc01EffectStartedTime;
					acc01TimeDiff = (acc01TimeDiff >= 2000) ? 2000 : acc01TimeDiff;
					_acc01Sum += acc01TimeDiff * 1200;
					_acc01EffectStarted = false;
				}

				// 
				if (_acc02EffectStarted)
				{
					UINT64 acc02TimeDiff = currentTime - _acc02EffectStartedTime;
					acc02TimeDiff = (acc02TimeDiff >= 10000) ? 10000 : acc02TimeDiff;
					_acc02Sum += acc02TimeDiff * 3000;
					_acc02EffectStarted = false;
				}

				// Save all to history
				(*player)->setHistoryBS(90, (DOUBLE)_gear90Sum / currentTime);
				(*player)->setHistoryBS(50, (DOUBLE)_gear50Sum / currentTime);
				(*player)->setHistoryBS(1, (DOUBLE)_acc01Sum / currentTime);
				(*player)->setHistoryBS(2, (DOUBLE)_acc02Sum / currentTime);
			}
		}

		_losedHp = 0.0;

		_avgABSum = 0;
		_avgABPreviousTime = 0;

		_avgBDSum = 0;
		_avgBDPreviousTime = 0;

		_avgASSum = 0;
		_avgASPreviousTime = 0;

		_gear90EffectStarted = false;
		_gear90Sum = 0;
		_gear90EffectStartedTime = 0;

		_gear50EffectStarted = false;
		_gear50Sum = 0;
		_gear50EffectStartedTime = 0;

		_acc01EffectStarted = false;
		_acc01Sum = 0;
		_acc01EffectStartedTime = 0;

		_acc02EffectStarted = false;
		_acc02Sum = 0;
		_acc02EffectStartedTime = 0;

		_fullABStarted = false;
		_fullABStartTime = 0;
		_fullABEndTime = 0;
		_fullABPrevTime = 0;
		_fullABTime = 0;

		_AggroStarted = false;
		_AggroStartTime = 0;
		_AggroEndTime = 0;
		_AggroPrevTime = 0;
		_AggroTime = 0;

		_fullASStarted = false;
		_fullASStartTime = 0;
		_fullASEndTime = 0;
		_fullASPrevTime = 0;
		_fullASTime = 0;
	}

	VOID HitEnemy() {
		if (!DAMAGEMETER.isRun()) {
			return;
		}
		UINT64 currentTime = DAMAGEMETER.GetTime();
		CalBsGear3Set(true, currentTime);
		CalBsGear4Set(true, currentTime);
		CalBsAccSet1(true, currentTime);
		CalBsAccSet2(true, currentTime);
	}

	// Add to sum if duration of effect is finished or effect duration is updated
	// If duration is not finished, calculate it based on table time
	UINT64 CalBsGear3Set(bool hit, UINT64 currentTime) {
		const double HP_CONDITION = 90.0;
		const int DURATION = 5000;
		const int ATK = 500;

		const DOUBLE hpPercent = (DOUBLE)_currentHP / (DOUBLE)_maxHP * 100;

		if (currentTime - _gear90EffectStartedTime < 0) {
			return 0;
		}

		UINT64 _gear90TimeLapse = currentTime - _gear90EffectStartedTime;

		if ((hpPercent >= HP_CONDITION) && hit && !_gear90EffectStarted) { // new start
			_gear90EffectStarted = true;
			_gear90EffectStartedTime = currentTime;
		}
		else if ((hpPercent >= HP_CONDITION) && hit && _gear90EffectStarted) { // update effect
			if (_gear90TimeLapse >= DURATION) {
				_gear90TimeLapse = DURATION;
			}
			_gear90Sum += _gear90TimeLapse * ATK;
			_gear90EffectStartedTime = currentTime;
		}
		else if ((hpPercent < HP_CONDITION) && hit && _gear90EffectStarted) { // end effect
			if (_gear90TimeLapse >= DURATION) {
				_gear90TimeLapse = DURATION;
				_gear90Sum += _gear90TimeLapse * ATK;
				_gear90EffectStarted = false;
			}
		}
		else if (!hit && !_gear90EffectStarted) { // PlayerTable
			return 0;
		}
		else if (!hit && _gear90EffectStarted && (_gear90TimeLapse >= 0)) { // PlayerTable
			if (_gear90TimeLapse >= DURATION) {
				_gear90TimeLapse = DURATION;
			}
			return _gear90TimeLapse * ATK;
		}
		return 0;
	}

	UINT64 CalBsGear4Set(bool hit, UINT64 currentTime) {
		const double HP_CONDITION = 50.0;
		const int DURATION = 5000;
		const int ATK = 1000;

		const DOUBLE hpPercent = (DOUBLE)_currentHP / (DOUBLE)_maxHP * 100;

		if (currentTime - _gear50EffectStartedTime < 0) {
			return 0;
		}

		UINT64 _gear50TimeLapse = currentTime - _gear50EffectStartedTime;

		if ((hpPercent >= HP_CONDITION) && hit && !_gear50EffectStarted) { // new start
			_gear50EffectStarted = true;
			_gear50EffectStartedTime = currentTime;
		}
		else if ((hpPercent >= HP_CONDITION) && hit && _gear50EffectStarted) { // update effect
			if (_gear50TimeLapse >= DURATION) {
				_gear50TimeLapse = DURATION;
			}
			_gear50Sum += _gear50TimeLapse * ATK;
			_gear50EffectStartedTime = currentTime;
		}
		else if ((hpPercent < HP_CONDITION) && hit && _gear50EffectStarted) { // end effect
			if (_gear50TimeLapse >= DURATION) {
				_gear50TimeLapse = DURATION;
				_gear50Sum += _gear50TimeLapse * ATK;
				_gear50EffectStarted = false;
			}
		}
		else if (!hit && !_gear50EffectStarted) { // PlayerTable
			return 0;
		}
		else if (!hit && _gear50EffectStarted && (_gear50TimeLapse >= 0)) { // PlayerTable
			if (_gear50TimeLapse >= DURATION) {
				_gear50TimeLapse = DURATION;
			}
			return _gear50TimeLapse * ATK;
		}
		return 0;
	}

	UINT64 CalBsAccSet1(bool hit, UINT64 currentTime) {
		const double HP_CONDITION = 85.0;
		const int DURATION = 2000;
		const int ATK = 1200;

		const DOUBLE hpPercent = (DOUBLE)_currentHP / (DOUBLE)_maxHP * 100;

		if (currentTime - _acc01EffectStartedTime < 0) {
			return 0;
		}

		UINT64 _acc01TimeLapse = currentTime - _acc01EffectStartedTime;


		if ((hpPercent >= HP_CONDITION) && hit && !_acc01EffectStarted) { // new start
			_acc01EffectStarted = true;
			_acc01EffectStartedTime = currentTime;
		}
		else if ((hpPercent >= HP_CONDITION) && hit && _acc01EffectStarted) { // update effect
			if (_acc01TimeLapse >= DURATION) {
				_acc01TimeLapse = DURATION;
			}
			_acc01Sum += _acc01TimeLapse * ATK;
			_acc01EffectStartedTime = currentTime;
		}
		else if ((hpPercent < HP_CONDITION) && hit && _acc01EffectStarted) { // end effect
			if (_acc01TimeLapse >= DURATION) {
				_acc01TimeLapse = DURATION;
				_acc01Sum += _acc01TimeLapse * ATK;
				_acc01EffectStarted = false;
			}
		}
		else if (!hit && !_acc01EffectStarted) { // PlayerTable
			return 0;
		}
		else if (!hit && _acc01EffectStarted && (_acc01TimeLapse >= 0)) { // PlayerTable
			if (_acc01TimeLapse >= DURATION) {
				_acc01TimeLapse = DURATION;
			}
			return _acc01TimeLapse * ATK;
		}
		return 0;
	}

	UINT64 CalBsAccSet2(bool hit, UINT64 currentTime) {
		const double HP_CONDITION = 30.0;
		const int DURATION = 10000;
		const int ATK = 3000;

		const DOUBLE hpPercent = (DOUBLE)_currentHP / (DOUBLE)_maxHP * 100;
		if (currentTime - _acc02EffectStartedTime < 0) {
			return 0;
		}

		UINT64 _acc02TimeLapse = currentTime - _acc02EffectStartedTime;

		if ((hpPercent < HP_CONDITION) && hit && !_acc02EffectStarted) { // new start
			_acc02EffectStarted = true;
			_acc02EffectStartedTime = currentTime;
		}
		else if ((hpPercent < HP_CONDITION) && hit && _acc02EffectStarted) { // update effect
			if (_acc02TimeLapse >= DURATION) {
				_acc02TimeLapse = DURATION;
			}
			_acc02Sum += _acc02TimeLapse * ATK;
			_acc02EffectStartedTime = currentTime;
		}
		else if ((hpPercent >= HP_CONDITION) && hit && _acc02EffectStarted) { // end effect
			if (_acc02TimeLapse >= DURATION) {
				_acc02TimeLapse = DURATION;
				_acc02Sum += _acc02TimeLapse * ATK;
				_acc02EffectStarted = false;
			}
		}
		else if (!hit && !_acc02EffectStarted) { // PlayerTable
			return 0;
		}
		else if (!hit && _acc02EffectStarted && (_acc02TimeLapse >= 0)) { // PlayerTable
			if (_acc02TimeLapse >= DURATION) {
				_acc02TimeLapse = DURATION;
			}
			return _acc02TimeLapse * ATK;
		}
		return 0;
	}

	VOID CalcFullABTime(UINT64 endTime = NULL)
	{
		if (endTime == NULL || _armorBreak < 100)
			endTime = _fullABEndTime;

		_fullABTime = _fullABPrevTime + ((DOUBLE)(endTime - _fullABStartTime) / 1000);
	}

	VOID CalcFullASTime(UINT64 endTime = NULL)
	{
		if (endTime == NULL || _attackSpeed < 250)
			endTime = _fullASEndTime;

		_fullASTime = _fullASPrevTime + ((DOUBLE)(endTime - _fullASStartTime) / 1000);
	}

	VOID CalcAggroTime(UINT64 endTime = NULL)
	{
		if (endTime == NULL || !_AggroStarted)
			endTime = _AggroEndTime;

		_AggroTime = _AggroPrevTime + ((DOUBLE)(endTime - _AggroStartTime) / 1000);
	}

	FLOAT GetStat(USHORT statType) {

		switch (statType) {
		case StatType::SG:
			return _sg;
		case StatType::Stamina:
			return _stamina;
		case StatType::SV:
			return _sv;
		case StatType::AttackSpeed:
			return _attackSpeed;
		case StatType::PartialDamage:
			return _partialDamage;
		case StatType::ArmorBreak:
			return _armorBreak;
		case StatType::MaxAttack:
			return _maxAttack;
		case StatType::CritDamage:
			return _critDamage;
		}

		return -1;
	}

	FLOAT GetSpecialStat(USHORT statType) {

		switch (statType) {
		case SpecialStatType::BossDamageAddRate:
			return _bossDamage;
		}

		return -1;
	}

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tPlayerMetaData>>& vPlayerMetaData)
	{
		auto fcsName = fbb.CreateString(_name);

		_tPlayerMetaDataBuilder tpmdb(fbb);

		tpmdb.add__armorbreak(GetStat(StatType::ArmorBreak));
		tpmdb.add__sg(GetStat(StatType::SG));
		tpmdb.add__stamina(GetStat(StatType::Stamina));
		tpmdb.add__sv(GetStat(StatType::SV));
		tpmdb.add__attackspeed(GetStat(StatType::AttackSpeed));
		tpmdb.add__partialdamage(GetStat(StatType::PartialDamage));
		tpmdb.add__maxhp(GetStat(StatType::MaxHP));
		tpmdb.add__currenthp(GetStat(StatType::CurrentHP));
		tpmdb.add__maxattack(GetStat(StatType::MaxAttack));
		tpmdb.add__critdamage(GetStat(StatType::CritDamage));

		tpmdb.add__bossdamage(GetSpecialStat(SpecialStatType::BossDamageAddRate));

		tpmdb.add__id(_id);
		tpmdb.add__name(fcsName);
		tpmdb.add__job(_job);

		vPlayerMetaData.push_back(tpmdb.Finish());
	}

	VOID UnSerialization(const _tPlayerMetaData* tPlayerMetaData)
	{
		_armorBreak = tPlayerMetaData->_armorbreak();
		_sg = tPlayerMetaData->_sg();
		_stamina = tPlayerMetaData->_stamina();
		_sv = tPlayerMetaData->_sv();
		_attackSpeed = tPlayerMetaData->_attackspeed();
		_partialDamage = tPlayerMetaData->_partialdamage();
		_maxHP = tPlayerMetaData->_maxhp();
		_currentHP = tPlayerMetaData->_currenthp();
		_maxAttack = tPlayerMetaData->_maxattack();
		_critDamage = tPlayerMetaData->_critdamage();
		_bossDamage = tPlayerMetaData->_bossdamage();

		_id = tPlayerMetaData->_id();
		strcpy_s(_name, tPlayerMetaData->_name()->c_str());
		_job = tPlayerMetaData->_job();
	}

}SW_PLAYER_METADATA;
private:
	vector<SWDamagePlayer*> _playerInfo;
	vector<SW_OWNER_ID_STRUCT*> _ownerInfo;
	vector<SW_DB2_STRUCT*> _dbInfo;
	unordered_map<UINT32, SW_PLAYER_METADATA*> _playerMetadata;

	vector<SWDamagePlayer*> _historyPlayerInfo;
	vector<SW_DB2_STRUCT*> _historyDbInfo;
	unordered_map<UINT32, SW_PLAYER_METADATA*> _historyPlayerMetadata;

	vector<string> _extInfo;
	unordered_map<UINT32, ULONG64> _playerUseAwaken;

	CHAR _mapName[MAX_MAP_LEN];
	UINT32 _myID;
	USHORT _worldID;

	USHORT _historyWorldID;
	ULONG64 _historyTime;
	UINT32 _historyMyID;
	LPVOID _historyHI;

	UINT32 _aggroedId;
	UINT32 _realClearTime;

	BOOL _mazeEnd;
	BOOL _historyMode;

	VOID InsertPlayerInfo(UINT32 id, UINT64 totalDMG, UINT64 soulstoneDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID);
	VOID Sort();
	VOID Restore();

	Timer _timer;

	mutex _mutex;

	INT32 _currentHistoryId = -1;

	BOOL _testMode = FALSE;

public:
	SWDamageMeter() :   _myID(0), _worldID(0), _mazeEnd(0), _historyMode(0), _historyWorldID(0), _historyTime(0) {}
	~SWDamageMeter();

	VOID GetLock();
	VOID FreeLock();

	VOID AddDamage(UINT32 id, UINT64 totalDMG, UINT64 soulstoneDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, USHORT maxCombo, UINT32 monsterID, UINT32 skillID);
	VOID AddPlayerGetDamage(UINT32 playerId, UINT64 totalDMG, SWPACKETDAMAGE_DAMAGETYPE damageType, UINT32 monsterID, UINT32 skillID);
	VOID AddEnlighten(UINT32 playerId, FLOAT value);
	VOID AddSkillUsed(UINT32 playerId, UINT32 skillId);
	VOID AddDodgeUsed(UINT32 playerId);
	VOID AddDeath(UINT32 playerId);
	VOID BuffIn(UINT32 playerId, USHORT buffId, BYTE stack, UINT32 giverId);
	VOID BuffOut(UINT32 playerId, USHORT buffId);

	VOID InsertOwnerID(UINT32 id, UINT32 owner_id);
	UINT32 GetOwnerID(UINT32 id);

	VOID InsertDB(UINT32 id, UINT32 db2);
	SW_DB2_STRUCT* GetMonsterDB(UINT32 id);

	VOID SetWorldID(USHORT worldID);
	USHORT GetWorldID();
	const CHAR* GetWorldName();
	BOOL isTownMap();

	VOID SetAggro(UINT32 id, UINT32 targetedId);
	UINT32 GetAggro();

	VOID InsertPlayerMetadata(UINT32 id, CHAR* str, BYTE job);
	const CHAR* GetPlayerName(UINT32 id);
	BYTE GetPlayerJob(UINT32 id);

	VOID UpdateSpecialStat(UINT32 id, USHORT statType, FLOAT statValue);
	VOID UpdateStat(UINT32 id, USHORT statType, FLOAT statValue, BOOL isSpecial = FALSE);

	VOID SetMyID(UINT32 id);

	UINT32 GetMyID(BOOL useHistoryID = FALSE);

	BOOL CheckPlayer(UINT32 id);

	vector<SWDamagePlayer*>* GetPlayerInfoByHistory();
	vector<SWDamagePlayer*>::const_iterator GetPlayerInfo(UINT32 id);
	vector<SWDamagePlayer*>::const_iterator begin();
	vector<SWDamagePlayer*>::const_iterator end();
	const SIZE_T size();

	unordered_map<UINT32, SW_PLAYER_METADATA*>* GetPlayerMetaDataByHistory();

	SW_PLAYER_METADATA* GetPlayerMetaData(UINT32 id);
	SW_PLAYER_METADATA* GetPlayerMetaDataIfNotExistsCreate(UINT32 id);

	UINT64 GetPlayerTotalDamage();

	BOOL isRun();

	VOID Suspend();
	VOID Start();
	VOID Clear();
	VOID Toggle();

	ULONG64 GetTime();

	VOID SetMazeState(BOOL end);

	VOID SetHistory(LPVOID hi);
	BOOL isHistoryMode();

	VOID ClearInfo(BOOL clear = FALSE);
	VOID ClearDB();

	INT32 GetCurrentHistoryId()
	{
		return _currentHistoryId;
	}

	VOID SetCurrentHistoryId(INT32 id)
	{
		_currentHistoryId = id;
	}

	VOID SetTestMode()
	{
		_testMode = TRUE;
	}

	VOID ClearTestMode()
	{
		_testMode = FALSE;
	}

	LPVOID GetHistoryHI()
	{
		return _historyHI;
	}

	VOID AddExtInfo(string str)
	{
		_extInfo.push_back(str);
	}

	VOID SetRealClearTime(UINT32 t)
	{
		_realClearTime = t;
	}

	VOID AddAwakenPlayer(UINT32 uPlayerId)
	{
		ULONG64 ulTime = GetTime();
		if (_playerUseAwaken.find(uPlayerId) != _playerUseAwaken.end())
			_playerUseAwaken[uPlayerId] = ulTime;
		else
			_playerUseAwaken.insert({ uPlayerId, ulTime });
	}

	BOOL PlayerInAwakening(UINT32 uPlayerId)
	{
		WORD wTimeOut = 10000;
		do
		{
			auto find = _playerUseAwaken.find(uPlayerId);
			if (find != _playerUseAwaken.end())
			{
				if (GetTime() <= find->second + wTimeOut)
					return TRUE;
			}

		} while (false);

		return FALSE;
	}
};