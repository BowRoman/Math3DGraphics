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

	mTimer.Initialize();

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 0.0f, -5.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));

	mConstantBuffer.Initialize();
	mMeshBuffer.Initialize(kVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);
	mVertexShader.Initialize(L"../Assets/Shaders/Transform.fx", Graphics::VertexPC::format);
	mPixelShader.Initialize(L"../Assets/Shaders/Transform.fx");
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();

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

	mTimer.Update();

	Input::InputSystem* iS = Input::InputSystem::Get();
	iS->Update();

	if (iS->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}

	Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();

	gs->BeginRender(Math::Vector4::Black());
	// rendering

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationZ(mTimer.GetTotalTime() * 10.0f);
	Math::Matrix4 viewMatrix;// = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(gs->GetAspectRatio());

	ConstantData data;
	data.wvp = worldMatrix * viewMatrix * projectionMatrix;
	data.wvp = Transpose(data.wvp);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	
	// bind input layout, and vertex/pixel shaders
	mVertexShader.Bind();
	mPixelShader.Bind();

	mMeshBuffer.Render();

	gs->EndRender();
}