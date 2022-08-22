#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Soulworker Packet\SWPacketBigParty.h"
#include ".\Damage Meter\Damage Meter.h"

SWPacketBigParty::SWPacketBigParty(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketBigParty::Do() {

	SWPACKETBIGPARTY_HEADER* pos_header = (SWPACKETBIGPARTY_HEADER*)(_data + sizeof(SWHEADER));

	BYTE* p_data = _data;
	p_data += sizeof(SWHEADER) + sizeof(SWPACKETBIGPARTY_HEADER);

	for (int i = 0; i < pos_header->_posPlayerCount; i++) {
		SWPACKETBIGPARTY_DATA* pos_data = (SWPACKETBIGPARTY_DATA*)p_data;
		BYTE job = *(p_data + sizeof(SWPACKETBIGPARTY_DATA) + pos_data->_nickSize + 1);

		WCHAR utf16[MAX_NAME_LEN] = { 0 };
		memcpy_s(utf16, MAX_NAME_LEN * sizeof(WCHAR), p_data + sizeof(SWPACKETBIGPARTY_DATA), pos_data->_nickSize);

		CHAR utf8[MAX_NAME_LEN] = { 0 };
		if (!UTF16toUTF8(utf16, utf8, MAX_NAME_LEN)) {
			//Log::WriteLog(const_cast<LPTSTR>(_T("Error in SWPacketBigParty : UTF16toUTF8 FAILED")));
			return;
		}

		DAMAGEMETER.InsertPlayerMetadata(pos_data->_playerID, utf8, job);

		p_data += sizeof(SWPACKETBIGPARTY_DATA) + pos_data->_nickSize + SWPACKETBIGPARTY_DUMMY;
	}
}

VOID SWPacketBigParty::Log() {

}

VOID SWPacketBigParty::Debug() {

}