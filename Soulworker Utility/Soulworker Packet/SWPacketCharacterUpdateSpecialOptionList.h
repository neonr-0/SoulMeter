#pragma once
#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"

#pragma pack(push, 1)
typedef struct _SWPacketCharacterUpdateSpecialOptionListHeader {

	UINT32 _playerID;
	BYTE _listCount;

}SWPacketCharacterUpdateSpecialOptionListHeader;

typedef struct _SWPacketCharacterUpdateSpecialOptionVal {

	USHORT _listID;
	FLOAT _listVal;

}SWPacketCharacterUpdateSpecialOptionVal;
#pragma pack(pop)

class SWPacketCharacterUpdateSpecialOptionList : public SWPacket {
protected:
	SWPacketCharacterUpdateSpecialOptionList() {}

public:
	SWPacketCharacterUpdateSpecialOptionList(SWHEADER* swheader, BYTE* data);
	~SWPacketCharacterUpdateSpecialOptionList() {}

	VOID Do();
	VOID Log();
	VOID Debug();
};