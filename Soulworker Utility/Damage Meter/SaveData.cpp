#include "pch.h"
#include ".\Damage Meter\SaveData.h"

SWSaveData::~SWSaveData()
{
	BOOL isLock = _mutex.try_lock();

	if (_saveFile.is_open())
		_saveFile.close();

	FreeLock();
}

DWORD SWSaveData::Init()
{
	DWORD error = ERROR_FILE_NOT_FOUND;

	do
	{
		// Open save data, set deny read/write
		while (TRUE)
		{
			_saveFile.open(_saveFileName, ios::in | ios::out | ios::binary, _SH_DENYRW);
			if (!_saveFile.is_open())
			{
				_saveFile.close();

				_saveFile.open(_saveFileName, ios::out, _SH_DENYRW);
				if (!_saveFile)
				{
					error = ERROR_FILE_SYSTEM_LIMITATION;
					break;
				}
				_saveFile.close();
			}
			else {
				error = ERROR_SUCCESS;
				break;
			}
		}

		if (!_inited)
		{
			if (!Load()) {
				error = ERROR_FILE_CORRUPT;
				break;
			}
		}

	} while (false);

	return error;
}

BOOL SWSaveData::Load()
{
	LONG64 fileSize = GetCurrentLength();
	// file version
	if (fileSize >= sizeof(_saveVersion))
	{
		LONG64 offset = 0;
		UINT32 fileVersion = 0;
		LONG64 processedHistory = 0;

		ReadData((UCHAR*)&fileVersion, sizeof(_saveVersion), offset);
		offset += sizeof(_saveVersion);

		// Check version
		if (fileVersion != _saveVersion)
		{
			return false;
		}

		// Unserialization
		while (fileSize > offset)
		{
			ReadSaveData(offset);
			processedHistory++;
		}

		_inited = TRUE;

		LONG64 clearCount = processedHistory - HISTORY_SIZE;
#if DEBUG_SAVEDATA_DELETE == 1
		Log::WriteLogA("[SWSaveData::Load] Loaded data = %llu", processedHistory);
#endif
		if (clearCount > 0)
			Delete(-1, clearCount);
	}
	else {
		_fileNotExist = TRUE;
	}

	return true;
}

VOID SWSaveData::ReadSaveData(LONG64& offset)
{
	LONG64 dataSize = GetNextSaveDataLength(offset);
	if (dataSize > 0)
	{
		UCHAR* pSaveData = new UCHAR[dataSize];

		ReadData(pSaveData, dataSize, offset);
		offset += dataSize;

		auto tHistory = Get_tHistory(pSaveData);
		HISTORY.UnSerialization(tHistory);
	}
}

VOID SWSaveData::Crypt(UCHAR* src, UCHAR* dest, LONG64 len)
{
	for (LONG64 i = 0; i < len; i++)
		dest[i] = src[i] ^ (12 + 14);
}

LONG64 SWSaveData::GetCurrentLength()
{
	LONG64 size = 0;

	_saveFile.seekg(0, std::ios::end);

	size = _saveFile.tellg();

	_saveFile.seekg(0, std::ios::beg);

	return size;
}

VOID SWSaveData::Save(flatbuffers::FlatBufferBuilder& fbb)
{
	GetLock();
	{
		if (_saveFile.is_open()) 
		{
			LONG64 size = fbb.GetSize();

			// go to end
			_saveFile.seekg(0, std::ios::end);

			// write save data version
			if (_fileNotExist)
			{
				_fileNotExist = FALSE;
				WriteData((UCHAR*)&_saveVersion, sizeof(UINT32));
			}

			// write savedata size
			WriteData((UCHAR*)&size, sizeof(LONG64));

			// write savedata
			WriteData((UCHAR*)fbb.GetBufferPointer(), size);

			// flush savedata
			_saveFile.flush();

#if DEBUG_SAVEDATA_SAVE == 1
			Log::WriteLogA("[SWSaveData::Save] Data saved, size = %llu", size);
#endif
		}

		FreeLock();
	}
}

VOID SWSaveData::Delete(LONG64 index, LONG64 clearCount)
{
	GetLock();
	{
		
		do
		{
			if (!_inited)
				break;

			LONG64 currentSize = GetCurrentLength();
			if (currentSize <= sizeof(_saveVersion))
				break;

			string tmpFileName = _saveFileName + string(".tmp");
			std::remove(tmpFileName.c_str());

			fstream tmpFile;
			tmpFile.open(tmpFileName, ios::out | ios::binary, _SH_DENYRW);

			WriteData((UCHAR*)&_saveVersion, sizeof(UINT32), &tmpFile);

			LONG64 offset = sizeof(_saveVersion);
			LONG64 i = 0;
			LONG64 prevSize = 0;
			UCHAR* tmpData = nullptr;
			while (currentSize > offset)
			{
				i++;

				LONG64 dataSize = GetNextSaveDataLength(offset);
				if (index != i && i > clearCount)
				{
					if (dataSize > prevSize)
					{
						delete[] tmpData;
						tmpData = new UCHAR[dataSize];
					}
					// read data from savedata
					ReadData(tmpData, dataSize, offset);

					// write datalength to tmp file
					WriteData((UCHAR*)&dataSize, sizeof(dataSize), &tmpFile);

					// write data to tmp file
					WriteData(tmpData, dataSize, &tmpFile);

					// flush tmp file
					tmpFile.flush();

					prevSize = dataSize;
				}
				offset += dataSize;
			}
#if DEBUG_SAVEDATA_DELETE == 1
			Log::WriteLogA("[SWSaveData::Delete] clearIndex = %llu, clearCount = %llu", index, clearCount);
#endif
			delete[] tmpData;

			_saveFile.close();
			tmpFile.close();

			std::remove(_saveFileName.c_str());
			if (std::rename(tmpFileName.c_str(), _saveFileName.c_str()))
				Log::WriteLogA("[SaveData::Delete] rename failed");

			Init();
		} while (false);

		FreeLock();
	}
}

VOID SWSaveData::WriteData(UCHAR* buf, LONG64 size, fstream* pFS)
{
	if (pFS == nullptr)
		pFS = &_saveFile;

	UCHAR* tmp = new UCHAR[size];

	Crypt(buf, tmp, size);
	
	pFS->write((const char*)tmp, size);

	delete[] tmp;
}

VOID SWSaveData::ReadData(UCHAR* buf, LONG64 size, LONG64 offset)
{
	_saveFile.seekg(offset, std::ios::beg);

	_saveFile.read((char*)buf, size);

	Crypt(buf, buf, size);
}

LONG64 SWSaveData::GetNextSaveDataLength(LONG64& offset)
{
	LONG64 dataSize = 0;

	ReadData((UCHAR*)&dataSize, sizeof(LONG64), offset);
	offset += sizeof(LONG64);

	return dataSize;
}