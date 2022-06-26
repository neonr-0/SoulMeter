#include "pch.h"
#include ".\Soulworker Packet\SWPacketCharacterUpdateSpecialOptionList.h"
#include ".\Buff Meter\Buff Meter.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\UI\PlotWindow.h"

SWPacketCharacterUpdateSpecialOptionList::SWPacketCharacterUpdateSpecialOptionList(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketCharacterUpdateSpecialOptionList::Do() {

	SHORT offset = sizeof(SWHEADER);
	SWPacketCharacterUpdateSpecialOptionListHeader* listPacket = (SWPacketCharacterUpdateSpecialOptionListHeader*)(_data + offset);
	offset += sizeof(SWPacketCharacterUpdateSpecialOptionListHeader);

	if (listPacket->_playerID != DAMAGEMETER.GetMyID())
		return;

	for (BYTE i = 0; i < listPacket->_listCount; i++)
	{
		SWPacketCharacterUpdateSpecialOptionVal* valPacket = (SWPacketCharacterUpdateSpecialOptionVal*)(_data + offset);
		offset += sizeof(SWPacketCharacterUpdateSpecialOptionVal);

		DAMAGEMETER.UpdateSpecialStat(listPacket->_playerID, valPacket->_listID, valPacket->_listVal);
	}
}

VOID SWPacketCharacterUpdateSpecialOptionList::Log() {

}

VOID SWPacketCharacterUpdateSpecialOptionList::Debug() {

	/*SHORT offset = sizeof(SWHEADER);
	SWPacketCharacterUpdateSpecialOptionListHeader* listPacket = (SWPacketCharacterUpdateSpecialOptionListHeader*)(_data + offset);
	offset += sizeof(SWPacketCharacterUpdateSpecialOptionListHeader);

	if (listPacket->_playerID != DAMAGEMETER.GetMyID())
		return;

	for (BYTE i = 0; i < listPacket->_listCount; i++)
	{
		SWPacketCharacterUpdateSpecialOptionVal* valPacket = (SWPacketCharacterUpdateSpecialOptionVal*)(_data + offset);
		offset += sizeof(SWPacketCharacterUpdateSpecialOptionVal);

		Log::WriteLogA("[SWPacketCharacterUpdateSpecialOptionList] listID = %d, listVal = %.f", valPacket->_listID, valPacket->_listVal);

	}*/
}