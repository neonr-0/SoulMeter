#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketOtherInfosMonster.h"

SWPacketOtherInfosMonster::SWPacketOtherInfosMonster(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketOtherInfosMonster::Do() {

	SHORT counts = *(_data + sizeof(SWHEADER));
	SHORT offset = sizeof(SWHEADER) + sizeof(counts);
	for (SHORT i = 0; i < counts; i++) {

		SWPACKET_OTHER_INFOS_MONSTER1* pktHeader = (SWPACKET_OTHER_INFOS_MONSTER1*)(_data + offset);
		offset += sizeof(SWPACKET_OTHER_INFOS_MONSTER1);
		if (pktHeader->data1Count > 0)
			offset += (pktHeader->data1Count * sizeof(SWPACKET_OTHER_INFOS_MONSTER_DATA1));

		DAMAGEMETER.InsertOwnerID(pktHeader->id, pktHeader->owner_id);
		DAMAGEMETER.InsertDB(pktHeader->id, pktHeader->realDB2);

		SWPACKET_OTHER_INFOS_MONSTER2* pktFooter = (SWPACKET_OTHER_INFOS_MONSTER2*)(_data + offset);
		offset += sizeof(SWPACKET_OTHER_INFOS_MONSTER2);
		if (pktFooter->data2Count > 0)
			offset += (pktFooter->data2Count * sizeof(SWPACKET_OTHER_INFOS_MONSTER_DATA2));
	}
}

VOID SWPacketOtherInfosMonster::Log() {

}


VOID SWPacketOtherInfosMonster::Debug() {

	/*SHORT counts = *(_data + sizeof(SWHEADER));
	SHORT offset = sizeof(SWHEADER) + sizeof(counts);
	for (SHORT i = 0; i < counts; i++) {

		SWPACKET_OTHER_INFOS_MONSTER1* pktHeader = (SWPACKET_OTHER_INFOS_MONSTER1*)(_data + offset);
		offset += sizeof(SWPACKET_OTHER_INFOS_MONSTER1);
		if (pktHeader->data1Count > 0)
			offset += (pktHeader->data1Count * sizeof(SWPACKET_OTHER_INFOS_MONSTER_DATA1));

		SWPACKET_OTHER_INFOS_MONSTER2* pktFooter = (SWPACKET_OTHER_INFOS_MONSTER2*)(_data + offset);
		offset += sizeof(SWPACKET_OTHER_INFOS_MONSTER2);
		if (pktFooter->data2Count > 0)
			offset += (pktFooter->data2Count * sizeof(SWPACKET_OTHER_INFOS_MONSTER_DATA2));

		Log::WriteLogA("MonsterID: %d, data1: %d, data2: %d", pktHeader->realDB2, pktHeader->data1Count, pktFooter->data2Count);
	}
	*/
}