#include "pch.h"
#include ".\Soulworker Packet\SWPacketMonsterStatUpdate.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\PacketInfo.h"
#include ".\Damage Meter\MySQLite.h"
#include ".\UI\PlayerTable.h"

SWPacketMonsterStatUpdate::SWPacketMonsterStatUpdate(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketMonsterStatUpdate::Do() {

	SHORT offset = sizeof(SWHEADER);
	SWPacketMonsterStatUpdatePkt* pkt = (SWPacketMonsterStatUpdatePkt*)(_data + offset);
	offset += sizeof(SWPacketMonsterStatUpdatePkt);
	for (BYTE i = 0; i < pkt->_statCounts; i++) {

		SWPacketMonsterStatData* pktStatData = (SWPacketMonsterStatData*)(_data + offset);
		offset += sizeof(SWPacketMonsterStatData);

		switch (pktStatData->_statID) {
		case StatType::CurrentHP:
			PLOTWINDOW.AddBossHpData(pkt->_id, pktStatData->_statVal, PLAYERTABLE.GetTableTime());
			break;
		}

	}

}

VOID SWPacketMonsterStatUpdate::Log() {

}

VOID SWPacketMonsterStatUpdate::Debug() {

	/*Log::WriteLogA("OP : %04x\tsize : %04x", _swheader->_op, _swheader->_size);
	for (int i = 0; i < _swheader->_size; i++)
		Log::WriteLogNoDate(L"%02x ", _data[i]);
	Log::WriteLogNoDate(L"\n");

	SHORT offset = sizeof(SWHEADER);
	SWPacketMonsterStatUpdatePkt* pkt = (SWPacketMonsterStatUpdatePkt*)(_data + offset);
	offset += sizeof(SWPacketMonsterStatUpdatePkt);
	for (BYTE i = 0; i < pkt->_statCounts; i++) {

		SWPacketMonsterStatData* pktStatData = (SWPacketMonsterStatData*)(_data + offset);
		offset += sizeof(SWPacketMonsterStatData);

		UCHAR monsterName[MONSTER_NAME_LEN] = { 0 };
		SW_DB2_STRUCT* monsterDB = DAMAGEMETER.GetMonsterDB(pkt->_id);
		if (monsterDB == nullptr)
			continue;

		switch (pktStatData->_statID) {
		case StatType::CurrentHP:
			PLOTWINDOW.AddBossHpData(pkt->_id, pktStatData->_statVal, PLAYERTABLE.GetTableTime());
			break;
		}

		Log::WriteLogA("[SWPacketMonsterStatUpdatePkt] monsterID: %04X, unk01:%.f, statID: %d, statVal: %llu",
			pkt->_id, 
			pktStatData->_unk01,
			pktStatData->_statID, pktStatData->_statVal
		);
	}

	Log::WriteLogNoDate(L"\n\n");*/
}