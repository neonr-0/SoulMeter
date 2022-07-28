#pragma once
#include "pch.h"

#define UTILLWINDOW UtillWindow::getInstance()

class UtillWindow : public Singleton<UtillWindow> {
private:
	bool _isOpen = false;

	ImFileDialogInfo _fileDialogInfo;
	bool _fileDialogOpen = false;

	INT32 _currentIndex = 0;


public:
	VOID OpenWindow();
	VOID Update();
	UtillWindow();
	~UtillWindow();

};