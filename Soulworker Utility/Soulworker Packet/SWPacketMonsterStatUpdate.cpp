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

	/*SHORT offset = sizeof(SWHEADER);
	SWPacketMonsterStatUpdatePkt* pkt = (SWPacketMonsterStatUpdatePkt*)(_data + offset);
	offset += sizeof(SWPacketMonsterStatUpdatePkt);

	Log::WriteLogA("[SWPacketMonsterStatUpdate] MonsterId = %u, StatCount = %d", pkt->_id, pkt->_statCounts);

	for (BYTE i = 0; i < pkt->_statCounts; i++) {

		SWPacketMonsterStatData* pktStatData = (SWPacketMonsterStatData*)(_data + offset);
		offset += sizeof(SWPacketMonsterStatData);

		Log::WriteLogNoDate(L"%u : %llu : %.3f\n", pktStatData->_statID, pktStatData->_statVal, pktStatData->_unk01);

	}

	Log::WriteLogNoDate(L"\n");*/
}