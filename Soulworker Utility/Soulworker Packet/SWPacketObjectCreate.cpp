#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketObjectCreate.h"

SWPacketObjectCreate::SWPacketObjectCreate(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketObjectCreate::Do() {

	SWPACKETOBJECTCREATE* obj_create = (SWPACKETOBJECTCREATE*)(_data + sizeof(SWHEADER));

	DAMAGEMETER.InsertOwnerID(obj_create->_id, obj_create->_owner_id);
	DAMAGEMETER.InsertDB(obj_create->_id, obj_create->_realDB2);
}

VOID SWPacketObjectCreate::Log() {

}


VOID SWPacketObjectCreate::Debug() {


	SWPACKETOBJECTCREATE* obj_create = (SWPACKETOBJECTCREATE*)(_data + sizeof(SWHEADER));

	//Log::MyLog(_T("unknown02\n"));
	//for (int i = 0; i < 17; i++)
	//	Log::MyLog(_T("%02x "), obj_create->_unknown02[i]);
	//Log::MyLog(_T("\n"));

	//Log::MyLog(_T("unknown03\n"));
	//for (int i = 0; i < 54; i++)
	//	Log::MyLog(_T("%02x "), obj_create->_unknown03[i]);
	//Log::MyLog(_T("\n"));

	//Log::WriteLogA(const_cast<CHAR*>("[DEBUG] [Created Object] [X = %f] [Y = %f] [Z = %f]"), obj_create->_locationX, obj_create->_locationY, obj_create->_locationZ);
	//Log::WriteLogA(const_cast<CHAR*>("[DEBUG] [Created Object] [X = %f] [Y = %f] [Z = %f]"), obj_create->_unknownFloat1, obj_create->_unknownFloat2, obj_create->_unknownFloat3);

	//Log::MyLog(const_cast<CHAR*>("[DEBUG] [Created Object] [ID = %u] [Owner ID = %08x] [DB2 = %u] [RealDB2 = %u]\n"), obj_create->_id, obj_create->_owner_id, obj_create->_db2, obj_create->_realDB2);

	//Log::WriteLogA(const_cast<CHAR*>("[DEBUG] [Created Object] [ID = %08x] [Owner ID = %08x] [DB2 = %u] [RealDB2 = %u]"), obj_create->_id, obj_create->_owner_id, obj_create->_db2, obj_create->_realDB2);
}