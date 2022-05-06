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
	else { // 한섭은 1이면 뭔가 안맞길래 이래해둠
		// when you entering maze, there is stat packets
#if defined(SERVER_KOREA)
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER) + 14;
#endif
#if defined(SERVER_STEAM)
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER);
#endif
#if defined(SERVER_JAPAN)
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER) + 14; // same as KR server
#endif
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
	else { // 한섭은 1이면 뭔가 안맞길래 이래해둠 (스팀섭에서 1이면 어떤지는 확인 안해봄)
		// when you entering maze, there is stat packets
#if defined(SERVER_KOREA)
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER) + 14;
#endif
#if defined(SERVER_STEAM)
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER);
#endif
#if defined(SERVER_JAPAN)
		p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETSTATCHANGE_HEADER) + 14; // same as KR server
#endif
	}

	for (int i = 0; i < stat_header->_statsCount; i++) {
		SWPACKETSTATCHANGE_DATA* party_data = (SWPACKETSTATCHANGE_DATA*)p_data;

		DAMAGEMETER.UpdateStat(stat_header->_playerID, party_data->_statType, party_data->_statValue);
		//Log::MyLog(_T("[DEBUG] [ID %08x] [statType = %x], [statValue = %f]\n"), stat_header->_playerID, party_data->_statType, party_data->_statValue);
		p_data += sizeof(SWPACKETSTATCHANGE_DATA);

	}
}