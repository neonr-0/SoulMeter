#pragma once
#include "pch.h"

#define PACKETPARSER PacketParser::getInstance()

#define NO_SEGMENTATION 0
#define YES_SEGMENTATION 1
#define REASSAMBLY 2
#define NO_SWHEADER 3
#define NO_SWDATA 4
#define SEGMENTATION_SUCCESS 5
#define SEGMENTATION_FAILED 6

#ifdef _DEBUG
#define DEBUG_SEGMENTATION 0
#define DEBUG_RESIZEPACKET 0
#endif

class PacketParser : public Singleton<PacketParser> {
private:
	class SegmentationPacket {
	private:
		BYTE* _data;
		SIZE_T _size;

		SIZE_T _curSize;
		SIZE_T _packetSize;

		IPv4Packet _packet;

		BOOL _isRecv;

	public:
		SegmentationPacket() {
			_data = nullptr;
			_size = 0;
			_curSize = 0;
			_packetSize = 0;
		}

		~SegmentationPacket() {

			if (_data != nullptr)
				delete[] _data;

			_data = nullptr;
		}

		VOID Init(SIZE_T size, BOOL isRecv) {

			if (_data == nullptr) {
				_data = new BYTE[size];
				_size = size;
			}

			if (_size < size) {
				delete[] _data;
				_data = new BYTE[size];
				_size = size;
			}

			_curSize = 0;
			_packetSize = size;
			_isRecv = isRecv;

#if DEBUG_SEGMENTATION == 1
			Log::WriteLogA(const_cast<CHAR*>("[%s] [segmentationPacket Init] [curSize = %d] [packetSize = %d]"), isRecv ? "RECV" : "SEND", _curSize, _packetSize);
#endif

		}

		SIZE_T GetRemainSize() {
#if DEBUG_SEGMENTATION == 1
			Log::WriteLogA(const_cast<CHAR*>("[%s] [segmentationPacket RemainSize = %d]"), _isRecv ? "RECV" : "SEND", _packetSize - _curSize);
#endif

			return _packetSize - _curSize;
		}

		DWORD AppendData(BYTE* data, SIZE_T size) {

			memcpy(_data + _curSize, data, size);

			SIZE_T temp_debug = _curSize;
			_curSize += size;

#if DEBUG_SEGMENTATION == 1
			Log::WriteLogA(const_cast<CHAR*>("[%s] [segmentationPacket AppendData Before curSize = %d After curSize = %d]"), _isRecv ? "RECV" : "SEND", temp_debug, _curSize);

			//for (int i = temp_debug; i < _curSize; i++)
			//	printf("%02x ", _data[i]);
			//printf("\n");
#endif

			if (_curSize == _packetSize) {
				return SEGMENTATION_SUCCESS;
			}
			else if (_curSize < _packetSize) {
				return SEGMENTATION_FAILED;
			}
			return SEGMENTATION_FAILED;
		}

		IPv4Packet* GetData() {
			_packet._data = _data;
			_packet._datalength = _packetSize;

			return &_packet;
		}
	};

	VOID ResizePacket(SIZE_T remainsize, IPv4Packet* packet);

	SegmentationPacket _segmentationRecv, _segmentationSend;
	BOOL _isSegmentationRecv, _isSegmentationSend;
	DWORD CheckSegmentation(IPv4Packet* packet, BOOL isRecv);
	VOID StartReassambly(IPv4Packet* packet, BOOL isRecv);
	BOOL Reassambly(IPv4Packet* packet, BOOL isRecv);
	VOID CheckRemainPacket(IPv4Packet* packet, BOOL isRecv);

public:
	PacketParser::PacketParser(): _isSegmentationRecv(FALSE), _isSegmentationSend(FALSE) {}

	VOID ResizePacket(IPv4Packet* packet, BOOL isRecv);

	DWORD Parse(IPv4Packet* packet, BOOL isRecv);
};