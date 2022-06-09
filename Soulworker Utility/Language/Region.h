#pragma once
// Supported Server : SERVER_KOREA, SERVER_STEAM, SERVER_JAPAN
// Supported Languages : _LANG_KOREAN, _LANG_ENGLISH, _LANG_CHINESES, _LANG_JAPANESE
#define SERVER_STEAM
#define _LANG_ENGLISH

#ifdef _LANG_KOREAN
#include "Korean.h"
#endif
#ifdef _LANG_ENGLISH
#include "English.h"
#endif
#ifdef _LANG_CHINESES
#include "Chineses.h"
#endif
#ifdef _LANG_JAPANESE
#include "Japanese.h"
#endif