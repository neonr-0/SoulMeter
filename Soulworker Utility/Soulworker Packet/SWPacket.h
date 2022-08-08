#pragma once

#pragma pack(push, 1)

typedef struct _SWHEADER { // 9
	USHORT _magic;
	USHORT _size;
	UCHAR _const_value01;
	UCHAR key;
	USHORT _op;
	UCHAR unk01;
} SWHEADER;

#pragma pack(pop)

class SWPacket : public MemoryPool<SWPacket, 10> {
protected:
	SWHEADER* _swheader;
	BYTE* _data;

	SWPacket() {}
	SWPacket(SWPacket& other) {}

public:
	SWPacket(SWHEADER* swheader, BYTE* data);
	virtual ~SWPacket() {}

	virtual VOID Debug();
	virtual VOID Log();
	virtual VOID Do();
};