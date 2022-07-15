#include "pch.h"
#include ".\Soulworker Packet\PacketType.h"
#include ".\Soulworker Packet\SWSPacketMaker.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\UI\PlayerTable.h"
#include ".\SWCrypt\SWCryptDLL.h"

SWSHEADER* SWSPacketMaker::GetSWSHeader(IPv4Packet* packet) {

	if (packet == nullptr || packet->_datalength < sizeof(SWSHEADER))
		return nullptr;

	SWSHEADER* swheader = (SWSHEADER*)(packet->_data);

	if (swheader->_magic != SWMAGIC || swheader->_const_value01 != SWCONSTVALUE)
		return nullptr;

	return swheader;

}

BYTE* SWSPacketMaker::GetSWSData(IPv4Packet* packet) {

	return (BYTE*)(packet->_data);
}

VOID SWSPacketMaker::Decrypt(BYTE* data, const UINT size, const UINT start, const UINT keyIndex) {
	if (data == nullptr || size < 0 || start < 0)
		return;
	UINT _size = size;

#if SWMAGIC == 3
	_size -= sizeof(SWHEADER) + 3;
	for (UINT i = 0; i < _size; i++) {
		data[i + start] ^= _keyTable[16 * (keyIndex % 16) + (i & 0xF)];
	}
#else
	SWCRYPT.SWDecrypt(data + start, size - start, keyIndex);
#endif
}

VOID SWSPacketMaker::CreateSWSPacket(IPv4Packet* packet) {

	SWSHEADER* swheader = GetSWSHeader(packet);
	BYTE* data = GetSWSData(packet);

	if (swheader == nullptr || data == nullptr)
		return;

	Decrypt(data, swheader->_size, sizeof(SWSHEADER) - 3, swheader->key);

	SWSPacket* swpacket = nullptr;
	DAMAGEMETER.GetLock();
	{
		switch ((SendOPCode)_byteswap_ushort(swheader->_op)) {
		case SendOPCode::HEARTBEAT:
			swpacket = new SWSPacketHeartbeat(swheader, data, packet->_ts);
			break;
		//case 0x0501:
		//	break;
		//case 0x0503:
		//	break;
		//case 0x0522:
		//	break;
		case SendOPCode::DODGE_USE:
			swpacket = new SWSPacketMyDodgeUsed(swheader, data);
			break;
		case SendOPCode::SKILL_USE:
			swpacket = new SWSPacketMySkillUsed(swheader, data);
			break;
		default:
			//swpacket = new SWSPacket(swheader, data);
			break;
		}

		if (swpacket != nullptr) {
#if DEBUG_SEND_CREATESPACKET == 1
			swpacket->Debug();
#endif
			// Todo
			swpacket->Do();

			delete swpacket;
		}
	}
	DAMAGEMETER.FreeLock();

	PACKETPARSER.ResizePacket(packet, FALSE);
}