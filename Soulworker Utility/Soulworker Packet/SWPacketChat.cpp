#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketChat.h"

SWPacketChat::SWPacketChat(SWHEADER* swheader, BYTE* data) : SWPacket(swheader, data) {

}

VOID SWPacketChat::Do() {
	SWPACKETCHAT_HEADER* chat_header = (SWPACKETCHAT_HEADER*)(_data + sizeof(SWHEADER));

	BYTE* p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETCHAT_HEADER);
}

VOID SWPacketChat::Log() {

}

VOID SWPacketChat::Debug() {
	
	//SWPACKETCHAT_HEADER* chat_header = (SWPACKETCHAT_HEADER*)(_data + sizeof(SWHEADER));

	//BYTE* p_data = _data + sizeof(SWHEADER) + sizeof(SWPACKETCHAT_HEADER);

	//WCHAR utf16[MAX_NAME_LEN] = { 0 };
	//memcpy_s(utf16, MAX_NAME_LEN * sizeof(WCHAR), p_data, chat_header->_chatSize);

	//CHAR utf8[MAX_NAME_LEN] = { 0 };
	//if (!UTF16toUTF8(utf16, utf8, MAX_NAME_LEN)) {
	//	//Log::WriteLog(const_cast<LPTSTR>(_T("Error in SWPacketChat : UTF16toUTF8 FAILED")));
	//	return;
	//}

	//Log::MyLog(const_cast<LPTSTR>(_T("[DEBUG] [CHAT] [ID %d] [ChatType = %d] [unknown1 = %02x] [unknown2 = %04x] [chatSize = %d] [chat = %s]\n")), 
	//chat_header->_playerID, chat_header->_chatType, chat_header->_unknown01, chat_header->_unknown02, chat_header->_chatSize, utf16);

}