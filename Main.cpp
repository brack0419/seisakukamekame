#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>

#include "Framework.h"

const LONG SCREEN_WIDTH = 1280;
const LONG SCREEN_HEIGHT = 720;

LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Framework* f = reinterpret_cast<Framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = fnWndProc;
	wcex.hInstance = instance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = _T("Game");

	RegisterClassEx(&wcex);

	HWND hWnd = nullptr;

#if defined(_DEBUG)

	// Debug用：固定ミニウィンドウ
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	int fixedX = 360;
	int fixedY = 0;

	hWnd = CreateWindow(
		_T("Game"),
		_T("Debug Window"),
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
		fixedX,
		fixedY,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		instance,
		NULL);

#else

	// Release用：フルスクリーン
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	hWnd = CreateWindow(
		_T("Game"),
		_T(""),
		WS_POPUP | WS_VISIBLE,
		0,
		0,
		screenW,
		screenH,
		NULL,
		NULL,
		instance,
		NULL);

#endif

	ShowWindow(hWnd, cmd_show);

	Framework f(hWnd);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));

	return f.Run();
}