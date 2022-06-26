#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Soulworker Packet\SWPacketStatChange.h"
#include ".\Damage Meter\Damage Meter.h"


SWPacketStatChange::SWPacketStatChange(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketStatChange::Do() {

	USHORT worldID = DAMAGEMETER.GetWorldID();
	if (worldID == 0 || worldID == 10003 || worldID == 10021 || worldID == 10031 || worldID == 10041 || worldID == 11001 || worldID == 10051 || worldID == 10061 || worldID == 20011) {
		return;
	}

	SWPACKETSTATCHANGE_HEADER* stat_header = (SWPACKETSTATCHANGE_HEADER*)(_data + sizeof(SWHEADER));

	BYTE* p_data;
	if (stat_header->_unknown01 == 0) {
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER);
	}
	else { // 
		// when you entering maze, there is stat packets
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER) + 14;
	}

	for (int i = 0; i < stat_header->_statsCount; i++) {
		SWPACKETSTATCHANGE_DATA* party_data = (SWPACKETSTATCHANGE_DATA*)p_data;
		//Log::MyLog(_T("[DEBUG] [ID %08x] [statType = %x], [statValue = %f]\n"), stat_header->_playerID, party_data->_statType, party_data->_statValue);
		DAMAGEMETER.UpdateStat(stat_header->_playerID, party_data->_statType, party_data->_statValue);
		p_data += sizeof(SWPACKETSTATCHANGE_DATA);
	}
}

VOID SWPacketStatChange::Log() {

}

VOID SWPacketStatChange::Debug() {
	SWPACKETSTATCHANGE_HEADER* stat_header = (SWPACKETSTATCHANGE_HEADER*)(_data + sizeof(SWHEADER));

	BYTE* p_data;
	if (stat_header->_unknown01 == 0) {
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER);
	}
	else { // 
		// when you entering maze, there is stat packets
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER) + 14;
	}

	for (int i = 0; i < stat_header->_statsCount; i++) {
		SWPACKETSTATCHANGE_DATA* party_data = (SWPACKETSTATCHANGE_DATA*)p_data;

		DAMAGEMETER.UpdateStat(stat_header->_playerID, party_data->_statType, party_data->_statValue);
		//Log::WriteLog(_T("[DEBUG] [ID %08x] [statType = %x], [statValue = %f]\n"), stat_header->_playerID, party_data->_statType, party_data->_statValue);
		p_data += sizeof(SWPACKETSTATCHANGE_DATA);

	}
}