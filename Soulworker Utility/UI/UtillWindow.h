#pragma once
#include "pch.h"

#define UTILLWINDOW UtillWindow::getInstance()

class UtillWindow : public Singleton<UtillWindow> {
private:
	bool _isOpen = false;

	ImFileDialogInfo _fileDialogInfo;
	bool _fileDialogOpen = false;

	CHAR _searchData[MAX_PATH] = { 0 };
	INT32 _currentIndex = 0;

	VOID handleDialogInfo();
public:
	VOID OpenWindow();
	VOID Update();
	UtillWindow();
	~UtillWindow();

};