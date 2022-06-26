#include "pch.h"
#include ".\Packet Capture\MyWinDivert.h"
#include ".\Packet Capture\PacketParser.h"
#include ".\Packet Capture\PacketCapture.h"
#include ".\UI\Option.h"

DWORD MyWinDivert::Init(HANDLE handle) {

	DWORD error = ERROR_SUCCESS;

	do {
		handle = WinDivertOpen(WINDIVERT_FILTER_RULE, WINDIVERT_LAYER_NETWORK, 0, WINDIVERT_FLAG_SNIFF);

		if (handle == INVALID_HANDLE_VALUE) {
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertOpen: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		if (!WinDivertSetParam(handle, WINDIVERT_PARAM_QUEUE_LEN, 16384))
		{
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertSetParam1: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		if (!WinDivertSetParam(handle, WINDIVERT_PARAM_QUEUE_TIME, 8000))
		{
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertSetParam2: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		if (!WinDivertSetParam(handle, WINDIVERT_PARAM_QUEUE_SIZE, 33554432))
		{
			Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertSetParam3: %x")), GetLastError());
			error = ERROR_INVALID_HANDLE;
			break;
		}

		_handle = handle;

		CreateThread(NULL, 0, ReceiveCallback, this, 0, NULL);

	} while (false);

	return error;
}

DWORD MyWinDivert::ReceiveCallback(LPVOID prc) {

	DWORD error = ERROR_SUCCESS;

	MyWinDivert* _this = (MyWinDivert*)prc;
	if (_this == nullptr)
		return ERROR_NOT_FOUND;

	do {
		WINDIVERT_ADDRESS addr;
		UINT addrlen = sizeof(WINDIVERT_ADDRESS);
		UINT recvlen = 0;

		while (TRUE) {

			BYTE* pkt_data = new BYTE[WINDIVERT_MTU_MAX];
			// Windivert 1.4.2
			if (!WinDivertRecvEx(_this->_handle, pkt_data, WINDIVERT_MTU_MAX, 0, &addr, &recvlen, NULL)) {
				// WinDivert 2.2
				//if (!WinDivertRecvEx(_this->_handle, pkt_data, packetlen, &recvlen, 0, &addr, &addrlen, NULL)) {

				Log::WriteLog(const_cast<LPTSTR>(_T("Error in WinDivertRecv : %x")), GetLastError());
				continue;
			}

			IPv4Packet* packet = new IPv4Packet;
			PACKETCAPTURE.ParseWinDivertStruct(packet, pkt_data);

			PacketInfo* pi = new PacketInfo;
			pi->_packet = packet;

			if (packet->_isRecv)
				PACKETCAPTURE.GetRecvQueue()->emplace(packet->_tcpHeader->seq_number, pi);
			else
				PACKETCAPTURE.GetSendQueue()->emplace(packet->_tcpHeader->seq_number, pi);
		}

	} while (false);

	return error;
}