#include "pch.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Soulworker Packet\SWPacketMaker.h"
#include ".\Soulworker Packet\SWSPacketMaker.h"
#include ".\Packet Capture\MyWinDivert.h"

VOID PacketParser::ResizePacket(IPv4Packet* packet, BOOL isRecv) {

	SIZE_T headerSize = 0;
	if (isRecv)
	{
		SWHEADER* swheader = SWPACKETMAKER.GetSWHeader(packet);
		if (swheader == nullptr)
			return;
		headerSize = swheader->_size;
	}
	else {
		SWSHEADER* swsheader = SWSPACKETMAKER.GetSWSHeader(packet);
		if (swsheader == nullptr)
			return;
		headerSize = swsheader->_size;
	}

#if DEBUG_RESIZEPACKET == 1
	Log::WriteLogA(const_cast<CHAR*>("[ResizePacket Before] [Packet Length = %d] [SWPacket Size = %u]"), packet->_datalength, swheader->_size);
#endif

	if (packet->_datalength >= headerSize) {
		packet->_datalength -= headerSize;
		packet->_data += headerSize;
	}
	else {
		packet->_datalength = 0;
	}

#if DEBUG_RESIZEPACKET == 1
	Log::WriteLogA(const_cast<CHAR*>("[ResizePacket After] [Packet Length = %d] [SWPacket Size = %u]"), packet->_datalength, swheader->_size);
#endif
}

VOID PacketParser::ResizePacket(SIZE_T remainsize, IPv4Packet* packet) {

#if DEBUG_RESIZEPACKET == 1
	Log::WriteLogA(const_cast<CHAR*>("[ResizePacket Before] [Packet Length = %d] [remainSize = %d]"), packet->_datalength, remainsize);
#endif

	if (packet->_datalength >= remainsize) {
		packet->_datalength -= remainsize;
		packet->_data += remainsize;
	}
	else {
		packet->_datalength = 0;
	}

#if DEBUG_RESIZEPACKET == 1
	Log::WriteLogA(const_cast<CHAR*>("[ResizePacket After] [Packet Length = %d] [remainSize = %d]"), packet->_datalength, remainsize);
#endif
}

DWORD PacketParser::CheckSegmentation(IPv4Packet* packet, BOOL isRecv) {

	BOOL* isSegmentation = nullptr;
	SIZE_T headerSize = 0;
	if (isRecv) {
		isSegmentation = &_isSegmentationRecv;
		SWHEADER* swheader = SWPACKETMAKER.GetSWHeader(packet);
		if (swheader != nullptr)
		{
			headerSize = swheader->_size;
		}
	}
	else {
		isSegmentation = &_isSegmentationSend;
		SWSHEADER* swsheader = SWSPACKETMAKER.GetSWSHeader(packet);
		if (swsheader != nullptr)
		{
			headerSize = swsheader->_size;
		}
	}

	if (*isSegmentation)
		return REASSAMBLY;

	if (headerSize <= 0)
		return NO_SWHEADER;

	if (packet->_datalength < headerSize) {
		*isSegmentation = TRUE;
		return YES_SEGMENTATION;
	}

	return NO_SEGMENTATION;
}

VOID PacketParser::StartReassambly(IPv4Packet* packet, BOOL isRecv) {

	SegmentationPacket* segmentationPacket = nullptr;
	BOOL* isSegmentation = nullptr;
	SIZE_T headerSize = 0;
	BYTE* data = nullptr;
	if (isRecv)
	{
		SWHEADER* swheader = SWPACKETMAKER.GetSWHeader(packet);
		if (swheader == nullptr)
			return;
		headerSize = swheader->_size;
		data = SWPACKETMAKER.GetSWData(packet);
		segmentationPacket = &_segmentationRecv;
	}
	else {
		SWSHEADER* swsheader = SWSPACKETMAKER.GetSWSHeader(packet);
		if (swsheader == nullptr)
			return;
		headerSize = swsheader->_size;
		data = SWSPACKETMAKER.GetSWSData(packet);
		segmentationPacket = &_segmentationSend;
	}

	if (data == nullptr)
		return;

	segmentationPacket->Init(headerSize, isRecv);
	segmentationPacket->AppendData(data, packet->_datalength);

	ResizePacket(packet, isRecv);
}

BOOL PacketParser::Reassambly(IPv4Packet* packet, BOOL isRecv) {

	SegmentationPacket* segmentationPacket = nullptr;
	BOOL* isSegmentation = nullptr;
	SIZE_T headerSize = 0;
	BYTE* data = nullptr;
	if (isRecv)
	{
		segmentationPacket = &_segmentationRecv;
		isSegmentation = &_isSegmentationRecv;
		data = SWPACKETMAKER.GetSWData(packet);
	}
	else {
		segmentationPacket = &_segmentationSend;
		isSegmentation = &_isSegmentationSend;
		data = SWSPACKETMAKER.GetSWSData(packet);
	}

	if (data == nullptr)
		return FALSE;

#if DEBUG_SEGMENTATION == 1
	Log::WriteLogA(const_cast<CHAR*>("[%s] [Reassambly] [datalength = %d]"), isRecv ? "RECV" : "SEND", packet->_datalength);
#endif

	SIZE_T remainsize = segmentationPacket->GetRemainSize();

	DWORD result = SEGMENTATION_SUCCESS;

	if (packet->_datalength >= remainsize) {
		result = segmentationPacket->AppendData(data, remainsize);
	}
	else if (packet->_datalength < remainsize) {
		result = segmentationPacket->AppendData(data, packet->_datalength);
	}

	ResizePacket(remainsize, packet);

	switch (result) {
	case SEGMENTATION_SUCCESS: {

#if DEBUG_SEGMENTATION == 1
		IPv4Packet* test = segmentationPacket->GetData();
		Log::WriteLogA(const_cast<CHAR*>("[%s] [Reassambly Success] [datalength = %d]"), isRecv ? "RECV" : "SEND", test->_datalength);

		//for (int i = 0; i < test->_datalength; i++)
		//	Log::WriteLogNoDate(L"%02x ", test->_data[i]);
		//Log::WriteLogNoDate(L"\n");
#endif
		if (isRecv)
			SWPACKETMAKER.CreateSWPacket(segmentationPacket->GetData());
		else
			SWSPACKETMAKER.CreateSWSPacket(segmentationPacket->GetData());

		*isSegmentation = FALSE;

		return TRUE;
	}
	case SEGMENTATION_FAILED:
		break;
	}
	return FALSE;
}

VOID PacketParser::CheckRemainPacket(IPv4Packet* packet, BOOL isRecv) {
	
	if (packet->_datalength > 0)
		Parse(packet, isRecv);
}

DWORD PacketParser::Parse(IPv4Packet* packet, BOOL isRecv) {

	if (packet == nullptr)
		return ERROR_INVALID_PARAMETER;

#if DEBUG_SEGMENTATION == 1
	Log::WriteLogA(const_cast<CHAR*>("[%s] [PacketParser::Parse] [length = %d] [seq = %lu]"), isRecv ? "RECV" : "SEND", packet->_datalength, packet->_tcpHeader->seq_number);
#endif

	switch (CheckSegmentation(packet, isRecv)) {
	case NO_SEGMENTATION:
		if (isRecv)
			SWPACKETMAKER.CreateSWPacket(packet);
		else
			SWSPACKETMAKER.CreateSWSPacket(packet);
		break;
	case YES_SEGMENTATION:
		StartReassambly(packet, isRecv);
		break;
	case REASSAMBLY:
		Reassambly(packet, isRecv);
		break;
	default:
		return ERROR_FUNCTION_FAILED;
	}

	CheckRemainPacket(packet, isRecv);

	return ERROR_SUCCESS;
}