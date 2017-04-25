#include "GameApp.h"

void GameApp::OnInitialize(uint32_t width, uint32_t height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
}

void GameApp::OnTerminate()
{
	Input::InputSystem::StaticTerminate();
	Graphics::GraphicsSystem::StaticTerminate();

	UnhookWindow();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
	}

	Input::InputSystem* iS = Input::InputSystem::Get();
	iS->Update();

	if (iS->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}

	Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();
	gs->BeginRender(Math::Vector4::Red());
	// rendering
	gs->EndRender();
}