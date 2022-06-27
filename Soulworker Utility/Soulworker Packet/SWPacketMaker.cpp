#include "pch.h"
#include ".\Soulworker Packet\PacketType.h"
#include ".\Soulworker Packet\SWPacketMaker.h"
#include ".\Soulworker Packet\SWCrypt.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\UI\PlayerTable.h"

SWHEADER* SWPacketMaker::GetSWHeader(IPv4Packet* packet) {

	if (packet == nullptr || packet->_datalength < sizeof(SWHEADER)) {
		return nullptr;
	}

	SWHEADER* swheader = (SWHEADER*)(packet->_data);

	if (swheader->_magic != SWMAGIC || swheader->_const_value01 != SWCONSTVALUE) {
		return nullptr;
	}

	return swheader;
}

BYTE* SWPacketMaker::GetSWData(IPv4Packet* packet) {

	return (BYTE*)(packet->_data);
}

VOID SWPacketMaker::Decrypt(BYTE* data, const UINT size, const UINT start, const INT32 keyIndex) {

	if (data == nullptr || size < 0 || start < 0)
		return;
	UINT32 _size = size;

#if SWMAGIC == 3
	_size -= sizeof(SWHEADER) + 3;
	BYTE ecx = keyIndex;
	for (UINT i = 0; i < _size; i++) {
		data[i + start] ^= _keyTable[16 * (ecx % 16) + (i & 0xF)];
	}
#else
	SWCrypt crypt;
	crypt.DecryptPacket(data + start, size - start, keyIndex);
#endif
}

VOID SWPacketMaker::CreateSWPacket(IPv4Packet* packet) {

	SWHEADER* swheader = GetSWHeader(packet);
	BYTE* data = GetSWData(packet);

	if (swheader == nullptr || data == nullptr)
		return;

	Decrypt(data, swheader->_size, sizeof(SWHEADER) - 3, swheader->key);

#if DEBUG_RECV_DISPLAY_ALL_PKT == 1
	Log::WriteLogA("OP : %04x\tsize : %04x", swheader->_op, swheader->_size);
	for (int i = 0; i < swheader->_size; i++)
		Log::WriteLogNoDate(L"%02x ", data[i]);
	Log::WriteLogNoDate(L"\n\n");
#endif

	SWPacket* swpacket = nullptr;
	DAMAGEMETER.GetLock();
	{
		switch (_byteswap_ushort(swheader->_op)) {
			/* 0x01*/
		case RecvOPcode::HEARTBEAT: 
			swpacket = new SWPacketHeartbeat(swheader, data, packet->_ts);
			break;

			/*0x03*/
		case RecvOPcode::STATCHANGE: //0430
			swpacket = new SWPacketStatChange(swheader, data);
			break;
		case RecvOPcode::DEAD:
			swpacket = new SWPacketDead(swheader, data);
			break;
		case RecvOPcode::CHARACTER_UPDATE_SPECIAL_OPTION_LIST:
			swpacket = new SWPacketCharacterUpdateSpecialOptionList(swheader, data);
			break;

			/*0x04*/
		case RecvOPcode::WORLDCHANGE: //0430
			swpacket = new SWPacketWorldChange(swheader, data);
			break;
		case RecvOPcode::MAZESTART:
			swpacket = new SWPacketMazeStart(swheader, data);
			break;
		case RecvOPcode::SPAWNED_CHARINFO:
			//swpacket = new SWPacketSpawnedCharInfo(swheader, data);
			break;
		case RecvOPcode::IN_INFO_MONSTER: //0605
			swpacket = new SWPacketInInfoMonster(swheader, data);
			break;
		case RecvOPcode::OTHER_INFOS_MONSTER:
			swpacket = new SWPacketOtherInfosMonster(swheader, data);
			break;

			/*0x05*/
		case RecvOPcode::STARTMOVE:
			//swpacket = new SWPacketStartMove(swheader, data);
			break;
		case RecvOPcode::STOPMOVE:
			break;
		case RecvOPcode::JUMP:
			break;
		case RecvOPcode::CANCEL_WITHMOVE:
			break;

			/*0x06 Combat*/
		//case RecvOPcode::EVADE:
		//	break;
		case RecvOPcode::USESKILL:
			//swpacket = new SWPacketUseSkill(swheader, data);
			break;
		case RecvOPcode::OTHER_USESKILL: //0430
			swpacket = new SWPacketOtherUseSkill(swheader, data);
			break;
		case RecvOPcode::DAMAGE: //0430
			swpacket = new SWPacketDamage(swheader, data);
			break;
		case RecvOPcode::BUFFIN: //0430
			swpacket = new SWPacketBuffIn(swheader, data);
			break;
		case RecvOPcode::BUFFOUT: //0430
			swpacket = new SWPacketBuffOut(swheader, data);
			break;
		case RecvOPcode::PROJECTILE:
			break;
		case RecvOPcode::AKASIC:
			swpacket = new SWPacketAkasic(swheader, data);
			break;
		case RecvOPcode::COOLDOWN: //0430
			swpacket = new SWPacketCooldown(swheader, data);
			break;

			/*0x07*/
		case RecvOPcode::CHAT:
			//swpacket = new SWPacketChat(swheader, data);
			break;

			/* 0x11 Maze*/
		case RecvOPcode::MAZEEND:
			swpacket = new SWPacketMazeEnd(swheader, data);
			break;
		case RecvOPcode::MAZE_UPDATE_STATE:
			swpacket = new SWPacketMazeUpdateState(swheader, data);
			break;

			/* 0x12 Party */
		case RecvOPcode::PARTY: //0430
			swpacket = new SWPacketParty(swheader, data);
			break;
		case RecvOPcode::PARTY_LIST_INFO:
			//swpacket = new SWPacketPartyListInfo(swheader, data);
			break;

			/* 0x17 Monster */
		case RecvOPcode::MONSTER_STAT_UPDATE:
			swpacket = new SWPacketMonsterStatUpdate(swheader, data);
			break;
		case RecvOPcode::MONSTER_KILLED: //0430
			swpacket = new SWPacketMonsterKilled(swheader, data);
			break;
		case RecvOPcode::AGGRO_CHANGED: //0430
			swpacket = new SWPacketAggroChanged(swheader, data);
			break;

			/* 0x23 Gesture*/
		case RecvOPcode::GESTURE_USED:
			//swpacket = new SWPacketGestureUsed(swheader, data);
			break;

			/* 0x2e Force*/
			// 8 players party
		case RecvOPcode::POS: //0430
			swpacket = new SWPacketPos(swheader, data);
			break;

		default:
#if DEBUG_RECV_DISPLAYPKT == 1
			Log::WriteLogA("OP : %04x\tsize : %04x", swheader->_op, swheader->_size);
			for (int i = 0; i < swheader->_size; i++)
				Log::WriteLogNoDate(L"%02x ", data[i]);
			Log::WriteLogNoDate(L"\n\n");
#endif
			break;
		}

		if (swpacket != nullptr) {
#if DEBUG_RECV_CREATEPACKET == 1
			swpacket->Debug();
#endif
			// Todo
			swpacket->Do();

			delete swpacket;
		}
	}
	DAMAGEMETER.FreeLock();

	PACKETPARSER.ResizePacket(packet, TRUE);
}