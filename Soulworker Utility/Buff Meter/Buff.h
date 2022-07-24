#pragma once
#include "pch.h"
#include ".\FlatBuffers\include\SW_HISTORY_.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define BUFF_NAME_LEN 64 // 

class Buff : public MemoryPool<Buff> {
private:
	USHORT _buffID;
	BYTE _stack;
	CHAR _name[BUFF_NAME_LEN];

	timePoint _timePoint;
	FLOAT _time;

	BOOL _isActive;

	BOOL _isHistoryMode = FALSE;

protected:
	Buff(const Buff& other) {}

public:
	Buff() {}
	Buff(USHORT buffID, BYTE stack);
	
	VOID Update();

	VOID Active();
	VOID InActive();

	const USHORT& GetBuffID();
	const BYTE& GetStack();
	const FLOAT& GetTime();
	const CHAR* GetName();

	VOID SetNameFromDB();

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tBuffInfo>>& vBuffInfo)
	{
		_tBuffInfoBuilder tbib(fbb);

		tbib.add__buff_id(GetBuffID());
		tbib.add__stack(GetStack());
		tbib.add__time(GetTime());

		vBuffInfo.push_back(tbib.Finish());
	}

	VOID UnSerialization(const _tBuffInfo* tBuffInfo)
	{
		_buffID = tBuffInfo->_buff_id();
		_stack = tBuffInfo->_stack();
		_time = tBuffInfo->_time();

		SetNameFromDB();

		_isHistoryMode = TRUE;
	}
};