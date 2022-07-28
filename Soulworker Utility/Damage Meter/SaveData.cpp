#include "pch.h"
#include ".\Damage Meter\SaveData.h"

SWSaveData::~SWSaveData()
{
	BOOL isLock = _mutex.try_lock();

	if (_saveFile.is_open())
		_saveFile.close();

	FreeLock();
}

VOID SWSaveData::Reset()
{
	_inited = false;
	_saveFile.close();
	_saveFile.clear();
}

DWORD SWSaveData::Init(string fileName)
{
	DWORD error = ERROR_FILE_NOT_FOUND;

	do
	{
		if (fileName.size() == 0)
			_saveFileName = _oriSaveFileName;
		else
			_saveFileName = fileName;
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

		if (!_inited && error == ERROR_SUCCESS)
		{
			try {
				if (!Load()) {
					throw "Load failed";
				}
			}
			catch (...)
			{
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
	LONG64 processedHistory = 0;
	// file version
	if (fileSize >= sizeof(_saveVersion))
	{
		LONG64 offset = 0;
		UINT32 fileVersion = 0;

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

	}
	else {
		_fileNotExist = TRUE;
	}

	_inited = TRUE;

	if (processedHistory > 0)
	{
		LONG64 clearCount = processedHistory - HISTORY_SIZE;
#if DEBUG_SAVEDATA_DELETE == 1
		Log::WriteLogA("[SWSaveData::Load] Loaded data = %llu", processedHistory);
#endif
		if (clearCount > 0)
			Delete(-1, clearCount);
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

		HISTORY.UnSerialization(Get_tHistory(pSaveData));

		delete[] pSaveData;
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
	BOOL isLock = _mutex.try_lock();
	{
		do
		{
			if (_saveFile.is_open())
			{
				_saveFile.close();

				string tmpFileName = _saveFileName + string(".tmp");

				// remove tmp file
				std::remove(tmpFileName.c_str());

				// copy current save file to tmp
				std::filesystem::copy(_saveFileName, tmpFileName);

				// open tmp file
				std::fstream tmpFile;
				tmpFile.open(tmpFileName, ios::in | ios::out | ios::binary, _SH_DENYRW);
				if (!tmpFile.is_open())
				{
					Log::WriteLogA("[SWSaveData::Save] open tmp file failed");
					exit(1);
				}

				// go to end
				tmpFile.clear();
				tmpFile.seekp(0, std::ios::end);

				// write save data version
				if (_fileNotExist)
				{
					_fileNotExist = FALSE;
					WriteData((UCHAR*)&_saveVersion, sizeof(UINT32), &tmpFile);
				}

				// write savedata size
				LONG64 size = fbb.GetSize();
				WriteData((UCHAR*)&size, sizeof(LONG64), &tmpFile);

				// write savedata
				WriteData((UCHAR*)fbb.GetBufferPointer(), size, &tmpFile);

				// flush tmp file
				tmpFile.close();

				// delete old savedata and rename tmp file to savedata
				std::remove(_saveFileName.c_str());
				if (std::rename(tmpFileName.c_str(), _saveFileName.c_str()))
				{
					Log::WriteLogA("[SaveData::Save] rename failed");
					exit(1);
				}

				// reopen savedata
				Init(_saveFileName);

#if DEBUG_SAVEDATA_SAVE == 1
				Log::WriteLogA("[SWSaveData::Save] Data saved, size = %llu", size);
#endif
			}
		} while (false);

		if (isLock)
			FreeLock();
	}
}

VOID SWSaveData::Delete(LONG64 index, LONG64 clearCount)
{
	BOOL isLock = _mutex.try_lock();
	{
		
		do
		{
			if (!_inited || !_saveFile.is_open())
				break;

			LONG64 currentSize = GetCurrentLength();
			if (currentSize <= sizeof(_saveVersion))
				break;

			string tmpFileName = _saveFileName + string(".tmp");
			std::remove(tmpFileName.c_str());

			fstream tmpFile;
			tmpFile.open(tmpFileName, ios::out | ios::binary, _SH_DENYRW);
			if (!tmpFile.is_open())
			{
				Log::WriteLogA("[SWSaveData::Save] open tmp file failed");
				exit(1);
			}

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

			// delete old savedata and rename tmp file to savedata
			std::remove(_saveFileName.c_str());
			if (std::rename(tmpFileName.c_str(), _saveFileName.c_str()))
			{
				Log::WriteLogA("[SaveData::Delete] rename failed");
				exit(1);
			}

			// reopen savedata
			Init(_saveFileName);
		} while (false);

		if (isLock)
			FreeLock();
	}
}

VOID SWSaveData::Clone(string filename)
{
	BOOL isLock = _mutex.try_lock();
	{
		if (_saveFile.is_open())
		{
			_saveFile.close();

			std::filesystem::copy(_saveFileName, filename);

			Init(_saveFileName);
		}
		if (isLock)
			_mutex.unlock();
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