#pragma once

#define BUFF_NAME_LEN 64 // 원래 32임

class Buff : public MemoryPool<Buff> {
private:
	USHORT _buffID;
	BYTE _stack;
	CHAR _name[BUFF_NAME_LEN];

	timePoint _timePoint;
	FLOAT _time;

	BOOL _isActive;

protected:
	Buff() {}
	Buff(const Buff& other) {}

public:
	Buff(USHORT buffID, BYTE stack);
	
	VOID Update();

	VOID Active();
	VOID InActive();

	const USHORT& GetBuffID();
	const BYTE& GetStack();
	const FLOAT& GetTime();
	const CHAR* GetName();
};