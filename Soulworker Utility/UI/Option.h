#pragma once
#include "pch.h"
#include <unordered_map>

#define UIOPTION UiOption::getInstance()

#define OPTION_FILE_NAME "option.xml"
#define NETWORK_NAME_LEN 256

#ifdef _DEBUG
#define DEBUG_READ_XML 0
#define DEBUG_COLUMN_WIDTH 0
#endif

class UiOption : public Singleton<UiOption> {
private:
	ImVec4 _jobColor[11];
	ImVec4 _jobBasicColor[11];
	ImVec4 _activeColor[2];
	ImVec4 _outlineColor;
	ImVec4 _textColor;
	ImVec4 _windowBg;

	FLOAT _fontScale;
	BOOL ShowFontSelector();
	VOID ShowFeatures();

	FLOAT _columnFontScale;
	FLOAT _tableFontScale;
	BOOL _is1K;
	BOOL _is1M;
	BOOL _isSoloMode;
	BOOL _hideName;
	ImVec2 _cellPadding;
	FLOAT _framerate;
	FLOAT _windowBorderSize;
	FLOAT _windowWidth;
	FLOAT _refreshTime;
	BOOL _isTopMost;
	BOOL _teamTA_LF;
	INT32 _teamTA_LF_Mode = 1;
	CHAR _selectedLang[128] = { 0 };
	BOOL _isSoloRankMode;
	INT32 _captureMode;
	BOOL _isUseSaveData;
	BOOL _oriIsUseSaveData;
	CHAR _selectedInterface[MAX_PATH] = { 0 };
	BOOL _isDontSaveUnfinishedMaze;
	BOOL _isUpdateCheck;

	BOOL ShowTableOption();
	BOOL ShowHotkeySetting();
	VOID ShowLangSelector();
	VOID ChangeLang();

	VOID ShowCaptureModeSelector();
	VOID ShowTeamTALFSelector();
	VOID ShowInterfaceSelector();

	VOID Helper();

	BOOL GetOption();
	BOOL SetBasicOption();

	BOOL _open;

	BOOL _inited = false;

	unordered_map<string, string> _allLangList = LANGMANAGER.GetAllLangFile();

	map<string, string> _interfaceList;

public:
	UiOption();
	~UiOption();

	const BOOL& isOption();
	VOID OpenOption();
	VOID Update();
	VOID Init();

	const ImU32 GetJobColor(UINT index);
	const ImU32 GetOutlineColor();
	const FLOAT& GetFontScale();
	const FLOAT& GetColumnFontScale();
	const FLOAT& GetTableFontScale();
	const ImVec4& GetActiveColor();
	const ImVec4& GetInActiveColor();
	const ImVec4& GetWindowBGColor();
	const BOOL& is1K();
	const BOOL& is1M();
	const BOOL& isSoloMode();
	const BOOL& doHideName();
	const BOOL& isTopMost();
	const BOOL& isTeamTALF();
	const INT32& TeamTALFMode();
	const BOOL& isSoloRankMode();
	const INT32& GetCaptureMode();
	const BOOL& isUseSaveData();
	const CHAR* GetUseInterface();
	const BOOL& isDontSaveUnfinishedMaze();
	const BOOL& isUpdateCheck();

	BOOL ToggleTopMost();

	const FLOAT& GetFramerate();
	VOID SetFramerate(FLOAT i);

	const FLOAT& GetWindowWidth();
	VOID SetWindowWidth(const FLOAT& width);

	const FLOAT& GetRefreshTime();

	BOOL SaveOption(BOOL skipWarning = FALSE);
};