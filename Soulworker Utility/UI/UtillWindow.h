#pragma once
#include "pch.h"
using namespace std;
#include <list>

#define UTILLWINDOW UtillWindow::getInstance()

class UtillWindow : public Singleton<UtillWindow> {
private:
	bool _isOpen = false;
	std::list<string> logList;

	CHAR CurrentDate[32] = { 0, };
public:
	VOID AddLog(string log);
	VOID OpenWindow();
	VOID Update();
	UtillWindow();
	~UtillWindow();

};