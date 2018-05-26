#include "GameApp.h"

GameApp::GameApp()
{}

GameApp::~GameApp()
{}

void GameApp::OnInitialize(uint32_t width, uint32_t height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mTimer.Initialize();

	mWorld.Initialize(1000u);
	mWorld.LoadLevel("../Assets/Templates/test_level.xml");
}

void GameApp::OnTerminate()
{
	mWorld.Terminate();

	Input::InputSystem::StaticTerminate();
	UnhookWindow();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	mTimer.Update();
	float dTime = mTimer.GetElapsedTime();

	Input::InputSystem* iS = Input::InputSystem::Get();
	iS->Update();

	if (iS->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}
	if (mWindow.ProcessMessage())
	{
		Kill();
	}
}