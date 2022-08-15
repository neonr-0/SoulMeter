#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketInInfoMonster.h"
#include ".\Combat Meter\CombatMeter.h"

SWPacketInInfoMonster::SWPacketInInfoMonster(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketInInfoMonster::Do() {

	BYTE counts = *(_data + sizeof(SWHEADER));
	SHORT offset = sizeof(SWHEADER) + sizeof(counts);
	for (BYTE i = 0; i < counts; i++) {

		SWPACKET_IN_INFO_MONSTER1* pktHeader = (SWPACKET_IN_INFO_MONSTER1*)(_data + offset);
		offset += sizeof(SWPACKET_IN_INFO_MONSTER1);
		if (pktHeader->data1Count > 0)
		{
			for (int i = 0; i < pktHeader->data1Count; i++)
			{
				SWPACKET_IN_INFO_MONSTER_DATA1* pMonsterData = (SWPACKET_IN_INFO_MONSTER_DATA1*)(_data + offset);

				CombatLog* pCombatLog = new CombatLog;
				pCombatLog->_type = CombatLogType::CHANGED_STATS;
				pCombatLog->_val1 = pMonsterData->type;
				pCombatLog->_val2 = pMonsterData->val;
				COMBATMETER.Insert(pktHeader->realDB2, CombatType::MONSTER, pCombatLog);

				offset += sizeof(SWPACKET_IN_INFO_MONSTER_DATA1);
			}
		}

		DAMAGEMETER.InsertOwnerID(pktHeader->id, pktHeader->owner_id);
		DAMAGEMETER.InsertDB(pktHeader->id, pktHeader->realDB2);

		SWPACKET_IN_INFO_MONSTER2* pktFooter = (SWPACKET_IN_INFO_MONSTER2*)(_data + offset);
		offset += sizeof(SWPACKET_IN_INFO_MONSTER2);
		if (pktFooter->data2Count > 0)
			offset += (pktFooter->data2Count * sizeof(SWPACKET_IN_INFO_MONSTER_DATA2));
	}
}

VOID SWPacketInInfoMonster::Log() {

}


VOID SWPacketInInfoMonster::Debug() {

	/*BYTE counts = *(_data + sizeof(SWHEADER));
	SHORT offset = sizeof(SWHEADER) + sizeof(counts);
	for (BYTE i = 0; i < counts; i++) {

		SWPACKET_IN_INFO_MONSTER1* pktHeader = (SWPACKET_IN_INFO_MONSTER1*)(_data + offset);
		offset += sizeof(SWPACKET_IN_INFO_MONSTER1);
		if (pktHeader->data1Count > 0)
			offset += (pktHeader->data1Count * sizeof(SWPACKET_IN_INFO_MONSTER_DATA1));

		SWPACKET_IN_INFO_MONSTER2* pktFooter = (SWPACKET_IN_INFO_MONSTER2*)(_data + offset);
		offset += sizeof(SWPACKET_IN_INFO_MONSTER2);
		if (pktFooter->data2Count > 0)
			offset += (pktFooter->data2Count * sizeof(SWPACKET_IN_INFO_MONSTER_DATA2));

		Log::WriteLogA("[SWPacketInInfoMonster] MonsterID: %u, DB2: %u, data1: %d, data2: %d", pktHeader->id, pktHeader->realDB2, pktHeader->data1Count, pktFooter->data2Count);
	}
	*/
}