#include "pch.h"

#include ".\UI\DX11.h"
#include ".\UI\DX Input.h"
#include ".\UI\UiWindow.h"
#include ".\UI\PlayerTable.h"
#include ".\UI\Option.h"
#include ".\UI\HotKey.h"
#include ".\UI\UtillWindow.h"
#include ".\UI\PlotWindow.h"
#include ".\UI\ModalWindow.h"
#include ".\Damage Meter\Damage Meter.h"
#include <io.h>
#include <chrono>
#include <thread>
#include ".\Packet Capture\PacketCapture.h"
#include ".\Packet Capture\MyNpcap.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace {
	UiWindow* uiWindow = 0;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	return uiWindow->WndProc(hWnd, msg, wParam, lParam);
}

UiWindow::UiWindow() : _x(0), _y(0), _width(0), _height(0), _swapChain(nullptr), _renderTargetView(nullptr), _hInst(0), _hWnd(0), _imGuiContext(nullptr), _deltaTime(0) {
	uiWindow = this;
}

UiWindow::~UiWindow() {

	if (_swapChain != nullptr) {
		_swapChain->Release();
		_swapChain = nullptr;
	}

	if (_renderTargetView != nullptr) {
		_renderTargetView->Release();
		_renderTargetView = nullptr;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

BOOL UiWindow::Init(UINT x, UINT y, UINT width, UINT height) {

	WNDCLASSEX wc;
	_hInst = GetModuleHandle(NULL);

	if (!GetClassInfoEx(_hInst, UI_WINDOW_CLASSNAME, &wc)) {
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = _hInst;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = UI_WINDOW_CLASSNAME;
		wc.hIconSm = NULL;
	}
	
	if (!RegisterClassEx(&wc)) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in RegisterClassEx")));
		return FALSE;
	}

	if ((_hWnd = CreateWindowEx(WS_EX_TOPMOST, wc.lpszClassName, _T(""), WS_POPUP, x, y, width, height, NULL, NULL, _hInst, NULL)) == NULL) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in CreateWindowEx : %x")), GetLastError());
		return FALSE;
	}
//	SetLayeredWindowAttributes(_hWnd, 0, 180, LWA_ALPHA);
//	SetLayeredWindowAttributes(_hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);


	_x = x; _y = y; _width = width, _height = height, _prevTimePoint = std::chrono::system_clock::now();

	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);

	if (!DIRECTX11.Init()) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in DirectX Init")));
		return FALSE;
	}

	if (!DXINPUT.Init(_hInst, _hWnd)) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in Direct Input Init")));
		return FALSE;
	}

	if ((_swapChain = DIRECTX11.CreateSwapChain(_hWnd)) == nullptr) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in CreateSwapChain")));
		return FALSE;
	}

	if ((_renderTargetView = DIRECTX11.CreateRenderTarget(_swapChain)) == nullptr) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in CreateRenderTarget")));
		return FALSE;
	}
	
	if (!InitImGUI()) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in Init ImGUI")));
		return FALSE;
	}
	
	return TRUE;
}

BOOL UiWindow::InitImGUI() {

	if (!IMGUI_CHECKVERSION())
		return FALSE;
	

	_imGuiContext = ImGui::CreateContext();
	ImPlot::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	//	style.Alpha = 0.5f;
	//	ImGui::SetNextWindowBgAlpha(0.5f);
	}

	style.WindowMinSize = ImVec2(20, 20);

	
	if (!ImGui_ImplWin32_Init(_hWnd))
		return FALSE;
	
	if (!ImGui_ImplDX11_Init(DIRECTX11.GetDevice(), DIRECTX11.GetDeviceContext()))
		return FALSE;

	SetFontList();
	UIOPTION.Init();

	return TRUE;
}

BOOL UiWindow::SetFontList() {

	_finddata_t fd;
	const char* path = ".\\Font\\";
	const char* filter = "*.ttf";
	vector<string> vsFontPathPool;

	string fontDir(path);
	fontDir.append(filter);

	auto handle = _findfirst(fontDir.c_str(), &fd);

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.OversampleH = 1;
	config.OversampleV = 1;

	if (handle == -1)
	{
		char szSysPath[MAX_PATH] = { 0 };
		if (GetWindowsDirectoryA(szSysPath, MAX_PATH) != 0)
		{
			const char* szWindowsFontsDir = "\\Fonts\\";
			string sFindDefaultFontPath(szSysPath);
			sFindDefaultFontPath.append(szWindowsFontsDir);
			sFindDefaultFontPath.append("msjh.*");

			_finddata_t defaultFontFD;
			auto pFont = _findfirst(sFindDefaultFontPath.c_str(), &defaultFontFD);

			if (pFont != -1)
			{
				string fnExt = defaultFontFD.name;
				if (fnExt.substr(fnExt.find_last_of(".") + 1) == "ttc" || fnExt.substr(fnExt.find_last_of(".") + 1) == "ttf")
				{
					sFindDefaultFontPath = szSysPath;
					sFindDefaultFontPath.append(szWindowsFontsDir);
					sFindDefaultFontPath.append(defaultFontFD.name);
					vsFontPathPool.push_back(sFindDefaultFontPath);
				}
			}
		}
	}
	else {
		do {
			char fontPath[MAX_BUFFER_LENGTH] = { 0 };
			strcat_s(fontPath, path);
			strcat_s(fontPath, fd.name);

			vsFontPathPool.push_back(fontPath);
		} while (_findnext(handle, &fd) != -1);

		_findclose(handle);
	}

	for (auto itr = vsFontPathPool.begin(); itr != vsFontPathPool.end(); itr++)
		io.Fonts->AddFontFromFileTTF((*itr).c_str(), 32, &config, io.Fonts->GetGlyphRangesChineseAndKoreaFull());

	return TRUE;
}

VOID UiWindow::Run() {

	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		
		Update();
	}
}

VOID UiWindow::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CalcDeltaTime();
	DXINPUT.Update();
	HOTKEY.Update();
	UIOPTION.Update();
	MODALWINDOW.Update();
	UTILLWINDOW.Update();
	PLOTWINDOW.Update();

	UpdateMainTable();
	CheckModalWindowState();
#ifdef _DEBUG
	//ImGui::ShowMetricsWindow();
#endif

	ImGui::EndFrame();
	DrawScene();
}

VOID UiWindow::DrawScene() {
	
	ImVec4 clear_color = UIOPTION.GetWindowBGColor();

	ImGui::Render();
	DIRECTX11.GetDeviceContext()->OMSetRenderTargets(1, &_renderTargetView, NULL);
	DIRECTX11.GetDeviceContext()->ClearRenderTargetView(_renderTargetView, (FLOAT*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	_swapChain->Present(static_cast<UINT>(UIOPTION.GetFramerate()), 0);
}

LRESULT UiWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return TRUE;

	switch (msg) {
	case WM_SIZE:
		_width = LOWORD(lParam);
		_height = HIWORD(lParam);
		OnResize();
		break;
	case WM_KILLFOCUS:
		UIOPTION.SetFramerate(4);
		break;
	case WM_SETFOCUS:
		UIOPTION.SetFramerate(1);
		break;
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		UIOPTION.SaveOption(TRUE);
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

VOID UiWindow::OnResize() {
	
	if (_swapChain == nullptr)
		return;
	
	if (_renderTargetView != nullptr) {
		_renderTargetView->Release();
		_renderTargetView = nullptr;
	}

	_swapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if ((_renderTargetView = DIRECTX11.CreateRenderTarget(_swapChain)) == nullptr) {
		Log::WriteLog(const_cast<LPTSTR>(_T("Error in CreateRenderTarget")));
		exit(-1);
	}
}

VOID UiWindow::CalcDeltaTime() {
	std::chrono::duration<FLOAT> deltaTime = std::chrono::system_clock::now() - _prevTimePoint;
	_prevTimePoint = std::chrono::system_clock::now();
	_deltaTime = deltaTime.count();
}

VOID UiWindow::UpdateMainTable() {
	PLAYERTABLE.Update();
}

VOID UiWindow::CheckModalWindowState() {
	
	if (MODALWINDOW.GetUID() == MODALWINDOW_UID_UPDATE)
		if (MODALWINDOW.GetState() == 1)
		{
			MODALWINDOW.SetUID(-1);
			CHAR errorMsg[256] = { 0 };
			// Download update
			httplib::Client cli("https://api.github.com");
			//httplib::Client cli("http://localhost");
			auto res = cli.Get("/repos/neonr-0/SoulMeter/releases/latest");
			//auto res = cli.Get("/latest.json");
			if (res != nullptr && res.error() == httplib::Error::Success && res->status == 200)
			{
				try
				{
					json jsonData = json::parse(res->body);
					string url = jsonData["assets"][0]["browser_download_url"];
					httplib::Client cli2("https://github.com");
					//httplib::Client cli2("http://localhost");
					cli2.set_follow_location(true);
					url = url.substr(sizeof("https://github.com") - 1);
					//url = url.substr(sizeof("http://localhost") - 1);
					auto res2 = cli2.Get(url.c_str());
					if (res2 != nullptr && res2.error() == httplib::Error::Success && res2->status == 200)
					{
						std::ofstream out("update.zip", std::ios::binary | std::ios::app);
						out << res2->body;
						out.close();
						elz::extractZip("update.zip", "./tmp", "");
						std::filesystem::remove("./update.zip");
						std::filesystem::copy("./tmp/SoulMeter.exe", "./update.exe", std::filesystem::copy_options::update_existing);
						//std::filesystem::remove("./tmp/WinDivert64.sys"); // ignore locked windivert driver
						Sleep(1000);
						wchar_t wpath[2048];
						GetCurrentDirectoryW(2048, wpath);
						HINSTANCE hInst = ShellExecuteW(NULL, L"open", L"update.exe", L"-update", wpath, 1);
						exit(0);
					}
				}
				catch (const std::exception&)
				{
					sprintf_s(errorMsg, "Connection issue");
					MessageBoxA(NULL, errorMsg, "SoulMeter", MB_ICONERROR | MB_OK | MB_TOPMOST);
				}
			}
			else
			{
				sprintf_s(errorMsg, "Update failed");
				MessageBoxA(NULL, errorMsg, "SoulMeter", MB_ICONERROR | MB_OK | MB_TOPMOST);
			}
		}	
}

const HWND& UiWindow::GetHWND() {
	return _hWnd;
}

const FLOAT& UiWindow::GetDeltaTime() {
	return _deltaTime;
}