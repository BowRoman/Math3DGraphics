#include "Precompiled.h"

#include "Window.h"

#include "Debug.h"

using namespace Core;

LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	{
		DestroyWindow(handle);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(handle, message, wParam, lParam);
}


Window::Window()
	: mInstance(nullptr)
	, mWindow(nullptr)
{
}

Window::~Window()
{
}

void Window::Initialize(HINSTANCE instance, LPCSTR appName, int width, int height)
{
	mInstance = instance;
	mAppName = appName;


	// Every Windows Window requires at least one window object.
	// Three things are involved:
	// 1)	Register a window class.
	// 2)	Create a window object.
	// 3)	Retrieve and dispatch messages for this window.

	// Register class
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	VERIFY(RegisterClassExA(&wcex), "[Window] Failed to register window class.");

	// Compute the correct window dimension
	RECT rc = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) >> 1;
	const int top = (screenHeight - winHeight) >> 1;

	// Create window
	mWindow = CreateWindowA
	(
		appName,
		appName,
		WS_OVERLAPPEDWINDOW,
		left,
		top,
		winWidth,
		winHeight,
		nullptr,
		nullptr,
		instance,
		nullptr
	);

	ASSERT(mWindow != nullptr, "[Window] Failed to create window.");

	ShowWindow(mWindow, true);

	VERIFY(SetCursorPos(screenWidth >> 1, screenHeight >> 1), "[Window] Failed to set cursor position.");
}

void Window::Terminate()
{
	DestroyWindow(mWindow);

	VERIFY(UnregisterClassA(mAppName.c_str(), mInstance), "[Window] Failed to unregister window class.");

	mWindow = nullptr;
	mInstance = nullptr;
}

bool Window::ProcessMessage()
{
	MSG msg = { 0 };

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (WM_QUIT == msg.message);
}