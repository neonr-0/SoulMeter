#include "pch.h"
#include ".\UI\ModalWindow.h"

ModalWindow::ModalWindow() : _state(-1), _label(0), _text(0), _leftButtonLabel(0), _rightButtonLabel(0), _uid(-1)
{
	_state = -1;
	_showWindow = false;
	_uid = -1;

}
ModalWindow::~ModalWindow()
{

}
VOID ModalWindow::Init()
{

	_state = -1;
	_showWindow = false;
}
VOID ModalWindow::ShowWindow()
{
	_showWindow = true;
}
VOID ModalWindow::Update()
{
	if (_showWindow)
	{
		if(_label!=0)
			ImGui::Begin(_label, 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
		else
			ImGui::Begin(LANGMANAGER.GetText("STR_MODALWINDOW_TITLE_DEFAULT"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
		if (_text != 0)
			ImGui::Text(_text);
		
		if (ImGui::Button(LANGMANAGER.GetText("STR_MODALWINDOW_CONFIRM"))) {
			_state = 1;
			_showWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(LANGMANAGER.GetText("STR_MODALWINDOW_CANCEL"))) {
			_state = 0;
			_showWindow = false;
		}
		ImGui::End();
	}
}

const int& ModalWindow::GetState()
{
	return _state;
}
const int& ModalWindow::GetUID()
{
	return _uid;
}
VOID ModalWindow::SetUID(int uid)
{
	_uid = uid;
}
VOID ModalWindow::SetText(char* text)
{
	_text = text;
}
VOID ModalWindow::SetLabel(char* label)
{
	_label = label;
}