#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketSquad.h"

SWPacketSquad::SWPacketSquad(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketSquad::Do() {

	SWPACKETSQUAD_HEADER_P1* squad_name = (SWPACKETSQUAD_HEADER_P1*)(_data + sizeof(SWHEADER));
	BYTE* p_data = _data;
	p_data += sizeof(SWHEADER)+sizeof(squad_name->_unknown01);//12Unk01
	//Ignoring Squad name for now
	p_data = p_data + squad_name->_squadNameSize + sizeof(squad_name->_squadNameSize);
	//Ignoring Squad Lead name
	USHORT* _squadLeadNameSize = (USHORT*)p_data;
	p_data += sizeof(USHORT) + _squadLeadNameSize[0];
	_SWPACKETSQUAD_HEADER_P2* squad_header = (_SWPACKETSQUAD_HEADER_P2*)(p_data);

	p_data += sizeof(SWPACKETSQUAD_HEADER_P2);

	for (int i = 0; i < squad_header->_squadCurPlayerCount; i++) {
		SWPACKETSQUAD_DATA* squad_data = (SWPACKETSQUAD_DATA*)p_data;
		BYTE job = *(p_data + sizeof(SWPACKETSQUAD_DATA) + squad_data->_nickSize + 1);

		WCHAR utf16[MAX_NAME_LEN] = { 0 };
		memcpy_s(utf16, MAX_NAME_LEN * sizeof(WCHAR), p_data + sizeof(SWPACKETSQUAD_DATA), squad_data->_nickSize);

		CHAR utf8[MAX_NAME_LEN] = { 0 };
		if (!UTF16toUTF8(utf16, utf8, MAX_NAME_LEN)) {
			//Log::WriteLog(const_cast<LPTSTR>(_T("Error in SWPacketParty : UTF16toUTF8 FAILED")));
			return;
		}

		DAMAGEMETER.InsertPlayerMetadata(squad_data->_playerID, utf8, job);

		p_data += sizeof(SWPACKETSQUAD_DATA) + squad_data->_nickSize + SWPACKETSQUAD_DUMMY;
	}
}

VOID SWPacketSquad::Log() {

}

VOID SWPacketSquad::Debug() {


}