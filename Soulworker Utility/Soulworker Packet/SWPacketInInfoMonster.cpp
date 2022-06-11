#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketInInfoMonster.h"

SWPacketInInfoMonster::SWPacketInInfoMonster(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketInInfoMonster::Do() {

	BYTE counts = *(_data + sizeof(SWHEADER));
	SHORT offset = sizeof(SWHEADER) + sizeof(counts);
	for (BYTE i = 0; i < counts; i++) {

		SWPACKET_IN_INFO_MONSTER1* pktHeader = (SWPACKET_IN_INFO_MONSTER1*)(_data + offset);
		offset += sizeof(SWPACKET_IN_INFO_MONSTER1);
		if (pktHeader->data1Count > 0)
			offset += (pktHeader->data1Count * sizeof(SWPACKET_IN_INFO_MONSTER_DATA1));

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



}