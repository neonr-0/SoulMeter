#include "pch.h"
#include "Language.h"

VOID Language::SetCurrentLang(SUPPORTED_LANG langID)
{
	if (langID > SUPPORTED_LANG_LEN)
		assert(FALSE);

	_currentLang = langID;

	for (auto& itr : _allTextList) {

		for (auto& itr2 : itr._vector) {
			if (itr2._lang != _currentLang)
				continue;
			if (_textList.find(itr._enum) == _textList.end())
				_textList.emplace(itr._enum, itr2._text);
			else
				_textList[itr._enum] = itr2._text;
		}
	}

	if (sizeof(_allTextList) / sizeof(*_allTextList) != STR_ALL_LENGTH)
		assert(FALSE);
}

CHAR* Language::GetText(TEXT_ENUM_LIST text)
{
	if (text >= STR_ALL_LENGTH || _textList.find(text) == _textList.end())
		assert(FALSE);

	return _textList.at(text);
}

CHAR* Language::GetSqlLang()
{
	switch (_currentLang)
	{
	case ZH_TW:
		return "TC";
	case EN_US:
	default:
		return "EN";
	}
}

CHAR* Language::GetLangName(SUPPORTED_LANG langID)
{
	switch (langID)
	{
	case ZH_TW:
		return GetText(STR_OPTION_COMBO_LANG_ZH);
	case EN_US:
	default:
		return GetText(STR_OPTION_COMBO_LANG_EN);
	}
}