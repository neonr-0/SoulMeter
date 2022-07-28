#pragma once
#include "pch.h"
#include ".\Third Party\FlatBuffers\include\SW_HISTORY_.h"
#include ".\Damage Meter\History.h"

using namespace std;
using namespace SoulMeterFBS::History;

#define SAVEDATA SWSaveData::getInstance()

#ifdef _DEBUG
#define DEBUG_SAVEDATA_SAVE 0
#define DEBUG_SAVEDATA_DELETE 0
#endif

// XOR( [Loop [[History Size]-[History Data]]  ] )
class SWSaveData : public Singleton<SWSaveData> {

private:
	const UINT32 _saveVersion = 1;
	const string _oriSaveFileName = "SoulMeter.dat";
	string _saveFileName;
	std::fstream _saveFile;

	BOOL _fileNotExist = FALSE;
	BOOL _inited = FALSE;

	BOOL Load();
	VOID Crypt(UCHAR* src, UCHAR* dest, LONG64 len);

	VOID ReadSaveData(LONG64& offset);

	VOID WriteData(UCHAR* buf, LONG64 size, fstream* pFS = nullptr);
	VOID ReadData(UCHAR* buf, LONG64 size, LONG64 offset);
	LONG64 GetNextSaveDataLength(LONG64& offset);

	mutex _mutex;

public:
	~SWSaveData();

	DWORD Init(string fileName = "");
	LONG64 GetCurrentLength();
	VOID Save(flatbuffers::FlatBufferBuilder& fbb);
	VOID Delete(LONG64 index, LONG64 clearCount = -1);
	VOID Clone(string filename);
	VOID Reset();

	VOID GetLock()
	{
		_mutex.lock();
	}
	VOID FreeLock()
	{
		_mutex.unlock();
	}

	const string CurrentSaveDataPath()
	{
		return _saveFileName;
	}
};