#pragma once
#include "pch.h"
#define MODALWINDOW ModalWindow::getInstance()

#define MODALWINDOW_UID_UPDATE 1

class ModalWindow : public Singleton<ModalWindow> {
private:
	bool _showWindow;
	int _state;
	int _uid;
	char* _label;
	char* _text;
	char* _leftButtonLabel;
	char* _rightButtonLabel;
public:
	ModalWindow();
	~ModalWindow();

	
	VOID Init();
	VOID Update();
	VOID ShowWindow();
	VOID SetLabel(char* label);
	VOID SetText(char* text);
	VOID SetUID(int uid);

	const int& GetUID();
	const int& GetState();
};