#include "pch.h"
#include ".\Combat Meter\Combat.h"
#include ".\Combat Meter\CombatMeter.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\Damage Meter\Damage Meter.h"

VOID CombatMeter::Insert(UINT32 id, CombatType type, CombatLog* cl)
{
	COMBATMETER.GetLock();
	{
		BOOL success = FALSE;
		do
		{
			if (_historyMode || _isEnd || DAMAGEMETER.isTownMap())
				break;

			if (_ci == nullptr)
				_ci = new CombatInterface();

			if (type == CombatType::MONSTER)
			{
				INT32 type = 0;
				SW_DB2_STRUCT* db = DAMAGEMETER.GetMonsterDB(id);
				if (db == nullptr)
				{
					SWDB.GetMonsterType(id, &type);
				}
				else {
					type = db->_type;
				}

				// Don't write not boss mob
				if (type != 3 && type != 4)
					break;
			}

			_ci->Insert(id, type, cl);
			success = TRUE;
		} while (false);

		if (!success)
			delete cl;

		COMBATMETER.FreeLock();
	}
}

string CombatMeter::ConvertCombatLogVal(CombatLog* pCombatLog, CombatType type)
{
	char tmp[1024] = { 0 };
	char* out = nullptr;
	string outPut;

	sprintf_s(tmp, "STR_UTILLWINDOW_COMBAT_LOG_TYPE_%d", pCombatLog->_type);
	char* typeText = LANGMANAGER.GetText(tmp);

	switch (pCombatLog->_type)
	{
	case CombatLogType::CHANGED_STATS:
	case CombatLogType::CHANGED_SPECIAL_STATS:
	{
		if (pCombatLog->_type == CombatLogType::CHANGED_STATS)
			sprintf_s(tmp, "STR_STAT_TYPE_%d", static_cast<INT32>(pCombatLog->_val1));
		else
			sprintf_s(tmp, "STR_SPECIAL_STAT_TYPE_%d", static_cast<INT32>(pCombatLog->_val1));

		char* statTypeText = LANGMANAGER.GetText(tmp);

		char comma[128] = { 0 };
		TextCommmaIncludeDecimal(pCombatLog->_val2, sizeof(comma), comma);

		sprintf_s(tmp, "%s %s %s", statTypeText, typeText, comma);
		break;
	}
	case CombatLogType::USED_SKILL:
	{
		char skillName[128] = { 0 };
		SWDB.GetSkillName(static_cast<UINT32>(pCombatLog->_val1), skillName, sizeof(skillName));
		if (strcmp(skillName, "0") == 0)
			sprintf_s(skillName, "%.0f", pCombatLog->_val1);

		sprintf_s(tmp, "%s %s", typeText, skillName);
		break;
	}
	case CombatLogType::GIVE_DAMAGE_NORMAL:
	case CombatLogType::GIVE_DAMAGE_MISS:
	case CombatLogType::GIVE_DAMAGE_CRIT:
	{
		char comma1[128] = { 0 };
		TextCommmaIncludeDecimal(pCombatLog->_val1, sizeof(comma1), comma1);

		if (type == CombatType::PLAYER)
		{
			char comma2[128] = { 0 };
			TextCommmaIncludeDecimal(pCombatLog->_val2, sizeof(comma2), comma2);

			sprintf_s(tmp, "%s %s(+%s)", typeText, comma1, comma2);
		}
		else
		{
			sprintf_s(tmp, "%s %s %s", typeText, DAMAGEMETER.GetPlayerName(static_cast<UINT32>(pCombatLog->_val2)), comma1);
		}
		break;
	}
	case CombatLogType::TAKE_DAMAGE_NORMAL:
	case CombatLogType::TAKE_DAMAGE_MISS:
	case CombatLogType::TAKE_DAMAGE_CRIT:
	{
		char comma1[128] = { 0 };
		TextCommmaIncludeDecimal(pCombatLog->_val1, sizeof(comma1), comma1);

		sprintf_s(tmp, "%s %s", typeText, comma1);
		break;
	}
	case CombatLogType::BUFF_STARTED:
	case CombatLogType::BUFF_END:
	{
		char buffName[128] = { 0 };
		if (!SWDB.GetBuffName(static_cast<UINT32>(pCombatLog->_val1), buffName, sizeof(buffName), NULL, NULL))
			sprintf_s(buffName, "%.0f", pCombatLog->_val1);

		if (pCombatLog->_type == CombatLogType::BUFF_STARTED)
			sprintf_s(tmp, "%s %s(%d)", typeText, buffName, static_cast<UINT32>(pCombatLog->_val2));
		else
			sprintf_s(tmp, "%s %s", typeText, buffName);
		break;
	}
	case CombatLogType::INVINCIBLE_SET:
	case CombatLogType::INVINCIBLE_UNSET:
	{
		sprintf_s(tmp, "%s %s", DAMAGEMETER.GetPlayerName(static_cast<UINT32>(pCombatLog->_val1)), typeText);
		break;
	}
	default:
		sprintf_s(tmp, "Unk%d(%.0lf, %.0lf)", pCombatLog->_type, pCombatLog->_val1, pCombatLog->_val2);
		break;
	}

	return string(tmp);
}

string CombatMeter::GetName(Combat* pCombat)
{
	char tmp[1024] = { 0 };
	char* out = nullptr;

	switch (pCombat->GetType())
	{
	case CombatType::MONSTER:
		if (SWDB.GetMonsterName(pCombat->GetID(), tmp, sizeof(tmp)))
			out = tmp;
#if _DEBUG
		sprintf_s(tmp, "%s(%u)", tmp, pCombat->GetID());
#endif
		break;
	case CombatType::PLAYER:
		out = (char*)DAMAGEMETER.GetPlayerName(pCombat->GetID());
		break;
	}

	if (out == nullptr)
	{
		sprintf_s(tmp, "Unk(%u)", pCombat->GetID());
		out = tmp;
	}

	return string(out);
}

CombatLogType CombatMeter::ConvertDamageTypeForGiveDamage(SWPACKETDAMAGE_DAMAGETYPE damageType)
{
	if (damageType.CRIT)
		return CombatLogType::GIVE_DAMAGE_CRIT;
	else if (damageType.MISS)
		return CombatLogType::GIVE_DAMAGE_MISS;
	else
		return CombatLogType::GIVE_DAMAGE_NORMAL;
}

CombatLogType CombatMeter::ConvertDamageTypeForTakeDamage(SWPACKETDAMAGE_DAMAGETYPE damageType)
{
	if (damageType.CRIT)
		return CombatLogType::TAKE_DAMAGE_CRIT;
	else if (damageType.MISS)
		return CombatLogType::TAKE_DAMAGE_MISS;
	else
		return CombatLogType::TAKE_DAMAGE_NORMAL;
}