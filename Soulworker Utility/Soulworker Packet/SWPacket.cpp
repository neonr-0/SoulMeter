#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"

SWPacket::SWPacket(SWHEADER* swheader, BYTE* data) {
	_swheader = swheader;
	_data = data;
}

VOID SWPacket::Do() {
	return;
}

VOID SWPacket::Log() {
	return;
}

VOID SWPacket::Debug() {



	USHORT op = _byteswap_ushort(_swheader->_op);

	//if (op != 0x1711) {
	//	return;
	//}

	//if (op == 0x050c || op == 0x0508 || op == 0x0514 || op == 0x0513 || op == 0x0105 || op == 0x067d || op == 0x1812) {
	//	return;
	//}

	//if (op == 0x064b || op == 0x1212 || op == 0x1722 || op==0x1931) {
	//	return;
	//}

	//if (op == 0x0642 || op == 0x0649 || op==0x0369 || op == 0x0103 || op == 0x0706 || op==0x0371) {
	//	return;
	//}

	//if (op == 0x1711 || op == 0x1712 || op == 0x1152 || op == 0x0409) {
	//	return;
	//}

	//if (op == 0x040e || op == 0x0621 || op == 0x0347 || op == 0x1135 || op == 0x2222) {
	//	return;
	//}

	//if (op == 0x050f || op == 0x0413) {
	//	return;
	//}

	//if (op != 0x067d) {
	//	return;
	//}

	//if (op == 0x0601 || op== 0x0906 || op==0x0405 || op==0x0049) {
	//	return;
	//}

	//if (_swheader->_op != 0x3403) {
	//	return;
	//}

	//printf("OP : %04x\tsize : %04x\n", op, _swheader->_size);

	//
	//for (int i = 0; i < _swheader->_size; i++)
	//	printf("%02x ", _data[i]);
	//printf("\n");

	//Log::MyLog(_T("Unkown Packet : %04x\n"), op);
	//for (int i = sizeof(SWHEADER); i < _swheader->_size; i++)
	//	Log::MyLog(_T("%02x "), _data[i]);
	//Log::MyLog(_T("\n"));

	
	return;
}