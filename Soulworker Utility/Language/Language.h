#pragma once
#include "pch.h"
#include <unordered_map>

#define LANGMANAGER Language::getInstance()

class Language : public Singleton<Language>
{

private:
	unordered_map<string, string> _textList;
	CHAR _currentLang[128] = { 0 };
	vector<string> _notFoundText;

public:
	Language() : _currentLang("zh_tw.json") {}

	const CHAR _langFolder[6] = "Lang/";

	CHAR* GetCurrentLang()
	{
		return _currentLang;
	}
	DWORD SetCurrentLang(CHAR* langFile);
	CHAR* GetText(CHAR* text, unordered_map<string, string>* vector = nullptr);
	unordered_map<string, string> GetAllLangFile();
	auto GetLangFile(CHAR* langFile, BOOL outputERROR = TRUE);
	unordered_map<string, string> MapLangData(CHAR* langFile, BOOL useReplace = TRUE);
};