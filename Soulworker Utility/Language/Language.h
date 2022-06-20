#pragma once
#include "pch.h"
#include ".\Language\LanguageText.h"
#include <unordered_map>

#define LANGMANAGER Language::getInstance()

class Language : public Singleton<Language>
{

private:
	unordered_map<TEXT_ENUM_LIST, CHAR*> _textList;
	SUPPORTED_LANG _currentLang;

public:
	Language() : _currentLang(SUPPORTED_LANG::EN_US) {}

	SUPPORTED_LANG GetCurrentLang()
	{
		return _currentLang;
	}
	VOID SetCurrentLang(SUPPORTED_LANG langID);
	CHAR* GetText(TEXT_ENUM_LIST text);
	CHAR* GetSqlLang();
	CHAR* Language::GetLangName(SUPPORTED_LANG langID);
};