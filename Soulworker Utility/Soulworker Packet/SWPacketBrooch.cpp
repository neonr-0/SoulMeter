#include "pch.h"
#include ".\Soulworker Packet\SWPacketBrooch.h"
#include ".\Buff Meter\Buff Meter.h"
#include ".\Damage Meter\Damage Meter.h"

SWPacketBrooches::SWPacketBrooches(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketBrooches::Do() {
	SWPACKETBROOCH* brooch_trigger = (SWPACKETBROOCH*)(_data + sizeof(SWHEADER));
	if(brooch_trigger->_broochID == 0x0431)//Tera Fever
		DAMAGEMETER.AddFeverProc(brooch_trigger->_playerID);
}

VOID SWPacketBrooches::Log() {

}

VOID SWPacketBrooches::Debug() {

}