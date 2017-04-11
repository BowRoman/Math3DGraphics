#include "Precompiled.h"
#include "Application.h"

using namespace Core;

Application::Application()
	: mInstance(nullptr)
	, mWindow(nullptr)
	, mRunning(true)
{

}

Application::~Application()
{

}

void Application::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height)
{
	mInstance = instance;
	mAppName = appName;
	
	CoInitialize(nullptr);

	OnInitialize(width, height);
}

void Application::Terminate()
{
	OnTerminate();

	CoUninitialize();
}

void Application::HookWindow(HWND hWnd)
{
	mWindow = hWnd;
}

void Application::UnhookWindow()
{
	mWindow = nullptr;
}

void Application::Update()
{
	OnUpdate();
}