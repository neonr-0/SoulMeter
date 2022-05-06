#include "pch.h"
#include ".\Soulworker Packet\SWSPacket.h"

SWSPacket::SWSPacket(SWSHEADER* swheader, BYTE* data) {
	_swheader = swheader;
	_data = data;
}

VOID SWSPacket::Do() {
	return;
}

VOID SWSPacket::Log() {
	return;
}

VOID SWSPacket::Debug() {


	USHORT op = _byteswap_ushort(_swheader->_op);

	//if (op == 0x0601 || op == 0x0906 || op == 0x0405 || op == 0x0049) {
	//	return;
	//}

	//if (_swheader->_op != 0x3403) {
	//	return;
	//}

	//Log::MyLog(_T("Unkown Packet : %04x\n"), op);
	//for (int i = sizeof(SWSHEADER); i < _swheader->_size; i++)
	//	Log::MyLog(_T("%02x "), _data[i]);
	//Log::MyLog(_T("\n"));


	return;
}