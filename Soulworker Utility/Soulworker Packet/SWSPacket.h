#pragma once

#pragma pack(push, 1)

typedef struct _SWSHEADER {
	USHORT _magic;
	USHORT _size;
	UCHAR _const_value01;
	UCHAR key;
	USHORT _op;
	UCHAR unk01;
} SWSHEADER;

#pragma pack(pop)

class SWSPacket : public MemoryPool<SWSPacket, 10> {
protected:
	SWSHEADER* _swheader;
	BYTE* _data;

	SWSPacket() {}
	SWSPacket(SWSPacket& other) {}

public:
	SWSPacket(SWSHEADER* swheader, BYTE* data);
	virtual ~SWSPacket() {}

	virtual VOID Debug();
	virtual VOID Log();
	virtual VOID Do();
};