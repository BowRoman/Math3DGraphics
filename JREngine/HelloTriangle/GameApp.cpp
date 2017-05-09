#include "GameApp.h"

namespace
{
	// vertex data
	const Graphics::VertexPC kVertices[] =
	{
		{ Math::Vector3(-0.50f, +0.25f, 0.0f), Math::Vector4::Red() },
		{ Math::Vector3(-0.25f, +0.50f, 0.0f), Math::Vector4::Orange() },
		{ Math::Vector3(+0.00f, +0.25f, 0.0f), Math::Vector4(0.5f, 0.5f, 0.0f, 1.0f) },
		{ Math::Vector3(+0.25f, +0.50f, 0.0f), Math::Vector4::Lime() },
		{ Math::Vector3(+0.50f, +0.25f, 0.0f), Math::Vector4::Green() },
		{ Math::Vector3(+0.50f, -0.25f, 0.0f), Math::Vector4::Cyan() },
		{ Math::Vector3(-0.50f, -0.25f, 0.0f), Math::Vector4::Magenta() },
		{ Math::Vector3(-0.00f, -0.75f, 0.0f), Math::Vector4::Blue() },
	};
	const int kVertexCount = sizeof(kVertices) / sizeof(kVertices[0]);

	// index data
	const uint32_t kIndices[]
	{
		0, 1, 2,
		2, 3, 4,
		0, 4, 5,
		0, 5, 6,
		6, 5, 7
	};
	const int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);
}

GameApp::GameApp()
{}

GameApp::~GameApp()
{}

void GameApp::OnInitialize(uint32_t width, uint32_t height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mMesh.Initialize(kVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);
	mVertexShader.Initialize(L"../Assets/Shaders/DoColour.fx", Graphics::VertexPC::format);
	mPixelShader.Initialize(L"../Assets/Shaders/DoColour.fx");
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mMesh.Terminate();

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

	gs->BeginRender(Math::Vector4::Black());
	// rendering

	// bind input layout, and vertex/pixel shaders
	mVertexShader.Bind();
	mPixelShader.Bind();

	mMesh.Render();

	gs->EndRender();
}