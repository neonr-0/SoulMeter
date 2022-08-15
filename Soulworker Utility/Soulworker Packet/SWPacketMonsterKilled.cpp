#include "pch.h"
#include ".\Soulworker Packet\SWPacketMonsterKilled.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Combat Meter\CombatMeter.h"

SWPacketMonsterKilled::SWPacketMonsterKilled(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMonsterKilled::Do() {

	SWPACKETMONSTERKILLED* killed = (SWPACKETMONSTERKILLED*)(_data + sizeof(SWHEADER));
	DAMAGEMETER.SetAggro(killed->_killedId, 0xffffffff);

	if (DAMAGEMETER.isRun()) {
		SW_DB2_STRUCT* db = DAMAGEMETER.GetMonsterDB(killed->_killedId);
		if (db == nullptr)
			return;

		CombatLog* pCombatLog = new CombatLog;
		pCombatLog->_type = CombatLogType::CHANGED_STATS;
		pCombatLog->_val1 = StatType::CurrentHP;
		pCombatLog->_val2 = 0;
		COMBATMETER.Insert(db->_db2, CombatType::MONSTER, pCombatLog);

		// BSVH P3
		if (DAMAGEMETER.GetWorldID() == 22018) {
			if (db->_db2 == 31310103)
				DAMAGEMETER.Suspend();
		}
	}
}

VOID SWPacketMonsterKilled::Log() {

}

VOID SWPacketMonsterKilled::Debug() {

	/*SWPACKETMONSTERKILLED* killed = (SWPACKETMONSTERKILLED*)(_data + sizeof(SWHEADER));
	Log::WriteLogA("[SWPacketMonsterKilled] KillId = %u", killed->_killedId);*/

}