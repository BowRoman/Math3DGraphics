#include "Precompiled.h"
#include "Application.h"


using namespace Core;


Core::Application::Application()
	: mInstance(nullptr)
	, mWindow(nullptr)
	, mRunning(true)
{

}

Core::Application::~Application()
{

}

void Core::Application::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height)
{
	mInstance = instance;
	mAppName = appName;
	CoInitialize(nullptr);
	OnInitialize(width, height);
}

void Core::Application::Terminate()
{
	OnTerminate();

	CoUninitialize();
}

void Core::Application::HookWindow(HWND hWnd)
{
	mWindow = hWnd;
}

void Core::Application::UnhookWindow()
{
	mWindow = nullptr;
}

void Core::Application::Update()
{
	OnUpdate();
}
