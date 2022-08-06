#pragma once
#include "pch.h"

#define SWCRYPT_API __declspec(dllimport)

class ISWCrypt
{
public:
    virtual VOID Decrypt(UCHAR* packet, INT32 pktLen, INT32 keyIndex) = 0;
    virtual VOID Destory() = 0;
};

typedef ISWCrypt* (*CREATE_SWCRYPT) ();

#define SWCRYPT SWCrypt::getInstance()

class SWCrypt : public Singleton<SWCrypt>
{
private:
    ISWCrypt* _pSWCryptRecv = nullptr;
    ISWCrypt* _pSWCryptSend = nullptr;
    mutex _mutexRecv;
    mutex _mutexSend;
    BOOL _stop = FALSE;
public:

    DWORD LoadSWCrypt()
    {
        DWORD error = ERROR_SUCCESS;

        do
        {
            HINSTANCE hDLL = LoadLibrary(L"SWCrypt.dll");

            if (hDLL == NULL) {
                error = ERROR_NOT_FOUND;
                Log::WriteLogA("[LoadSWCrypt] LoadLibrary Failed %d", GetLastError());
                break;
            }
            else {
                CREATE_SWCRYPT pSWCreateCrypt = (CREATE_SWCRYPT)GetProcAddress(hDLL, "CreateSWCrypt");
                if (pSWCreateCrypt == NULL)
                {
                    error = ERROR_FAILED_DRIVER_ENTRY;
                    Log::WriteLogA("[LoadSWCrypt] GetProcAddress Failed %d", GetLastError());
                    break;
                }
                _pSWCryptRecv = pSWCreateCrypt();
                _pSWCryptSend = pSWCreateCrypt();
                if (_pSWCryptRecv == NULL || _pSWCryptSend == NULL) 
                {
                    error = ERROR_FAILED_DRIVER_ENTRY;
                    Log::WriteLogA("[LoadSWCrypt] CreateCryptPtr Failed %d", GetLastError());
                    break;
                }

            }
        } while (false);

        return error;
    }

    VOID SWDecrypt(UCHAR* packet, INT32 pktLen, INT32 keyIndex, BOOL isRecv)
    {
        ISWCrypt* pSWCrypt = nullptr;
        
        if (_stop)
            return;

        mutex* pMutex = nullptr;

        if (isRecv)
        {
            pSWCrypt = _pSWCryptRecv;
            pMutex = &_mutexRecv;
        }
        else
        {
            pSWCrypt = _pSWCryptSend;
            pMutex = &_mutexSend;
        }

        pMutex->lock();
        {
            pSWCrypt->Decrypt(packet, pktLen, keyIndex);

            pMutex->unlock();
        }
    }

    ~SWCrypt()
    {
        _stop = TRUE;
        BOOL a = _mutexRecv.try_lock();
        a = _mutexSend.try_lock();
        _mutexRecv.unlock();
        _mutexSend.unlock();
    }
};