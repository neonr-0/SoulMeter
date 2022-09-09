#include "pch.h"
#include ".\Buff Meter\Buff.h"
#include ".\Damage Meter\MySQLite.h"

Buff::Buff(USHORT buffID, BYTE stack) : _buffID(buffID), _stack(stack) {
	_timePoint = std::chrono::system_clock::now();
	_time = 0;

	SetNameFromDB();
}

VOID Buff::SetNameFromDB()
{
	ZeroMemory(&_desc, BUFF_DESC_LEN);
	SWDB.GetBuffName(_buffID, _name, BUFF_NAME_LEN, _desc, BUFF_DESC_LEN);

	CHAR stackName[8] = { 0 };
	sprintf_s(stackName, 8, "(%d)", _stack);
	strcat_s(_name, BUFF_NAME_LEN, stackName);
}

VOID Buff::Update() {
	if (_isHistoryMode)
		return;

	if (_isActive) {
		std::chrono::duration<FLOAT> time;
		time = std::chrono::system_clock::now() - _timePoint;
		_time += time.count();
		_timePoint = std::chrono::system_clock::now();
	}
}

VOID Buff::Active() {
	if (_isHistoryMode)
		return;

	_timePoint = std::chrono::system_clock::now();
	_isActive = TRUE;
}

VOID Buff::InActive() {
	if (_isHistoryMode)
		return;

	if (_isActive) {
		std::chrono::duration<FLOAT> time;
		time = std::chrono::system_clock::now() - _timePoint;
		_time += time.count();
		_timePoint = std::chrono::system_clock::now();
	}

	_isActive = FALSE;
}

const USHORT& Buff::GetBuffID() {
	return _buffID;
}

const BYTE& Buff::GetStack() {
	return _stack;
}

const FLOAT& Buff::GetTime() {

	Update();

	return _time;
}

const CHAR* Buff::GetName() {
	return _name;
}

const CHAR* Buff::GetDesc() {
	return _desc;
}