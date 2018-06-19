#include "GameApp.h"

namespace
{
	// vertex data
	const Graphics::VertexPT kVertices[] =
	{
		{ Math::Vector3(-5.0f, -5.0f, 0.0f), Math::Vector2(0.f,1.f) },			// 0
		{ Math::Vector3(-5.0f, +5.0f, 0.0f), Math::Vector2(0.f,0.f) },			// 1
		{ Math::Vector3(+5.0f, +5.0f, 0.0f), Math::Vector2(1.f,0.f) },			// 2
		{ Math::Vector3(+5.0f, -5.0f, 0.0f), Math::Vector2(1.f,1.f) }			// 3

		/*
			1---2
			|   |
			0---3
		*/
	};
	const int kVertexCount = sizeof(kVertices) / sizeof(kVertices[0]);

	// index data
	const uint32_t kIndices[]
	{
		// front
		0, 1, 2,
		2, 3, 0

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
	Graphics::SimpleDraw::StaticInitialize(10000);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 2.0f, -5.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));

	mConstantBuffer.Initialize();

	mTerrainVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mTerrain.Initialize("../Assets/Images/heightMap.raw", 1024, 1024);

	mTerrainTexture.Initialize(L"../Assets/Images/terrain.jpg");
	mTerrainSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
}

void GameApp::OnTerminate()
{
	mTerrainSampler.Terminate();
	mTerrainTexture.Terminate();
	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mTerrain.Terminate();

	mConstantBuffer.Terminate();

	Input::InputSystem::StaticTerminate();
	Graphics::SimpleDraw::StaticTerminate();
	Graphics::GraphicsSystem::StaticTerminate();

	UnhookWindow();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	if(mWindow.ProcessMessage())
	{
		Kill();
	}

	mTimer.Update();

	Input::InputSystem* iS = Input::InputSystem::Get();
	iS->Update();

	if(iS->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}

	// camera control
	float deltaTime = mTimer.GetElapsedTime();
	const float boostModifyer = 1.6;
	const float cameraMoveSpeed = iS->IsKeyDown(Keys::LSHIFT) ? 10.f : 10.f*boostModifyer;
	const float cameraTurnSpeed = 0.7f;
	if (iS->IsKeyDown(Keys::W))
	{
		mCameraTransform.Walk(cameraMoveSpeed * deltaTime);
	}
	if (iS->IsKeyDown(Keys::S))
	{
		mCameraTransform.Walk(-cameraMoveSpeed * deltaTime);
	}
	if (iS->IsKeyDown(Keys::A))
	{
		mCameraTransform.Strafe(-cameraMoveSpeed * deltaTime);
	}
	if (iS->IsKeyDown(Keys::D))
	{
		mCameraTransform.Strafe(cameraMoveSpeed * deltaTime);
	}
	if (iS->IsKeyDown(Keys::E))
	{
		mCameraTransform.Rise(cameraMoveSpeed * deltaTime);
	}
	if (iS->IsKeyDown(Keys::Q))
	{
		mCameraTransform.Rise(-cameraMoveSpeed * deltaTime);
	}
	if (iS->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(iS->GetMouseMoveX() * cameraTurnSpeed * deltaTime);
		mCameraTransform.Pitch(iS->GetMouseMoveY() * cameraTurnSpeed * deltaTime);
	}

	Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();

	gs->BeginRender(Math::Vector4::Black());
	// rendering

	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(gs->GetAspectRatio());

	// bind input layout, and vertex/pixel shaders

	Math::Matrix4 matScale = Math::Matrix4::Scaling(1.f, 1.f, 1.f);
	Math::Matrix4 worldMatrix = matScale;

	ConstantData data;
	data.wvp = Transpose(worldMatrix *viewMatrix * projectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();
	mTerrainSampler.BindPS(0);
	mTerrainTexture.BindPS(0);
	mTerrain.Render();

	for (int z = 0; z <= 100; ++z)
	{
		Math::Vector3 p0(-50.f, 0.f, -50.f + z);
		Math::Vector3 p1(+50.f, 0.f, -50.f + z);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Lime());
	}
	for (int x = 0; x <= 100; ++x)
	{
		Math::Vector3 p0(-50.f + x, 0.f, -50.f);
		Math::Vector3 p1(-50.f + x, 0.f, +50.f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Lime());
	}

	Graphics::SimpleDraw::Flush(viewMatrix*projectionMatrix);

	gs->EndRender();
}