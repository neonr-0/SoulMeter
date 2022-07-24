#pragma once
#include "pch.h"
#include ".\Buff Meter\Buff.h"
#include ".\FlatBuffers\include\SW_HISTORY_.h"

using namespace SoulMeterFBS::History;

#define BUFFMETER BuffMeter::getInstance()

typedef struct _PLAYERBUFF {
private:
	UINT32 _playerID;
	vector<Buff*> _buffInfo;
	BOOL _isHistoryMode = FALSE;

protected:
	_PLAYERBUFF(const _PLAYERBUFF& other) {}

public:
	_PLAYERBUFF() {}
	_PLAYERBUFF(UINT32 playerID, USHORT buffid, BYTE stack);
	~_PLAYERBUFF();

	VOID AddBuff(USHORT buffid, BYTE stack);
	VOID EndBuff(USHORT buffid, BOOL endAll = FALSE);

	const UINT32& GetPlayerID();

	vector<Buff*>::const_iterator begin();
	vector<Buff*>::const_iterator end();

	VOID Serialization(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tBuff>>& vBuff)
	{
		// _tBuffInfo
		std::vector<flatbuffers::Offset<_tBuffInfo>> vBuffInfo;
		for (auto itr = begin(); itr != end(); itr++)
			(*itr)->Serialization(fbb, vBuffInfo);
		auto fcvBuffInfo = fbb.CreateVector(vBuffInfo);

		// _tBuff
		_tBuffBuilder tbb(fbb);
		tbb.add__id(_playerID);
		tbb.add__buff_info(fcvBuffInfo);

		vBuff.push_back(tbb.Finish());
	}
	VOID UnSerialization(const _tBuff* tBuff)
	{
		// _tBuffInfo
		for (auto itr = tBuff->_buff_info()->begin(); itr != tBuff->_buff_info()->end(); itr++)
		{
			Buff* newBuff = new Buff;
			newBuff->UnSerialization(*itr);

			_buffInfo.push_back(newBuff);
		}

		// _tBuff
		_playerID = tBuff->_id();

		_isHistoryMode = TRUE;
	}
}PLAYERBUF;

class BuffMeter : public Singleton<BuffMeter> {
private:
	vector<PLAYERBUF*> _playerBuffInfo;

	BOOL _historyMode = false;

	mutex _mutex;

public:
	BuffMeter();
	~BuffMeter();

	std::vector<PLAYERBUF*> GetPlayerInfo();
	VOID SetPlayerInfo(std::vector<PLAYERBUF*> it);
	VOID Clear();

	VOID AddBuff(UINT32 playerID, USHORT buffid, BYTE stack);
	VOID EndBuff(UINT32 playerID, USHORT buffid, BOOL endAll = FALSE);
	VOID EndAllBuff();

	vector<PLAYERBUF*>::const_iterator find(UINT32 playerID);
	vector<PLAYERBUF*>::const_iterator begin();
	vector<PLAYERBUF*>::const_iterator end();

	VOID GetLock();
	VOID FreeLock();
};