#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Soulworker Packet\SWPacketStatChange.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Combat Meter\Combat.h"
#include ".\Combat Meter\CombatMeter.h"

SWPacketStatChange::SWPacketStatChange(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketStatChange::Do() {

	if (DAMAGEMETER.isTownMap()) {
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

		if (party_data->_statType >= StatType::END)
		{
#if _DEBUG
			Log::WriteLogA("[SWPacketStatChange] Find Unknown statType = %x, statValue = %f", party_data->_statType, party_data->_statValue);
#endif
		}
		else {
			//Log::MyLog(_T("[DEBUG] [ID %08x] [statType = %x], [statValue = %f]\n"), stat_header->_playerID, party_data->_statType, party_data->_statValue);
			DAMAGEMETER.UpdateStat(stat_header->_playerID, party_data->_statType, party_data->_statValue);

			CombatLog* pCombatLog = new CombatLog;
			pCombatLog->_type = CombatLogType::CHANGED_STATS;
			pCombatLog->_val1 = party_data->_statType;
			pCombatLog->_val2 = party_data->_statValue;
			COMBATMETER.Insert(stat_header->_playerID, CombatType::PLAYER, pCombatLog);
		}

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