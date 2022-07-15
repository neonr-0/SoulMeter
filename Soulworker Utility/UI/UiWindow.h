#pragma once

#define UI_WINDOW_CLASSNAME _T("UI_WINDOW_CLASSNAME")
#define IMGUI_OPTION_FILE "option.xml"

#define UIWINDOW UiWindow::getInstance()

class UiWindow : public Singleton<UiWindow> {
private:
	IDXGISwapChain* _swapChain;
	ID3D11RenderTargetView* _renderTargetView;

	HINSTANCE _hInst;
	HWND _hWnd;

	UINT _x;
	UINT _y;
	UINT _width;
	UINT _height;

	ImGuiContext* _imGuiContext;

	timePoint _prevTimePoint;
	FLOAT _deltaTime;

	VOID DrawScene();
	VOID Update();
	VOID OnResize();
	VOID CalcDeltaTime();

	BOOL InitImGUI();
	BOOL SetFontList();

	VOID UpdateMainTable();

	BOOL _isExit = false;

public:
	UiWindow();
	~UiWindow();
	 
	VOID Run();
	BOOL Init(UINT x, UINT y, UINT width, UINT height);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const HWND& GetHWND();
	const FLOAT& GetDeltaTime();
};