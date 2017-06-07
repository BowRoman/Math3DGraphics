#include "GameApp.h"

namespace
{
	// vertex data
	const Graphics::VertexPC kVertices[] =
	{
		{ Math::Vector3(-1.0f, +1.0f, -1.0f), Math::Vector4::Red() },					// 0
		{ Math::Vector3(+1.0f, +1.0f, -1.0f), Math::Vector4::Orange() },				// 1
		{ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector4::Yellow() },				// 2
		{ Math::Vector3(+1.0f, -1.0f, -1.0f), Math::Vector4::Lime() },					// 3
																						
		{ Math::Vector3(-1.0f, +1.0f, +1.0f), Math::Vector4::Green() },					// 4
		{ Math::Vector3(+1.0f, +1.0f, +1.0f), Math::Vector4::Cyan() },					// 5
		{ Math::Vector3(-1.0f, -1.0f, +1.0f), Math::Vector4::Magenta() },				// 6
		{ Math::Vector3(+1.0f, -1.0f, +1.0f), Math::Vector4::Blue() },					// 7

		/*
			   4------5
			  /|     /|
			 / 6----/-7
			0------1 /
		    |/     |/
			2------3
		*/
	};
	const int kVertexCount = sizeof(kVertices) / sizeof(kVertices[0]);

	// index data
	const uint32_t kIndices[]
	{
		// front
		2, 0, 1,
		1, 3, 2,

		// back
		5, 4, 6,
		6, 7, 5,

		// top
		0, 4, 5,
		5, 1, 0,

		// bottom
		7, 6, 2,
		2, 3, 7,

		// left
		4, 0, 2,
		2, 6, 4,

		// right
		1, 5, 7,
		7, 3, 1

	};
	const int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);

	// basic sphere variables
	uint32_t numRings = 6, numSlices = 6;
	float scale = 1.f;
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

	// Basic Sphere control
	if (iS->IsKeyDown(Keys::O))
	{
		if(numRings > 0)
			--numRings;
	}
	if (iS->IsKeyDown(Keys::P))
	{
			++numRings;
	}
	if (iS->IsKeyDown(Keys::L))
	{
			++numSlices;
	}
	if (iS->IsKeyDown(Keys::K))
	{
		if(numSlices > 0)
			--numSlices;
	}
	if (iS->IsKeyDown(Keys::ZERO))
	{
		scale += 0.1f;
	}
	if (iS->IsKeyDown(Keys::NINE))
	{
		scale -= 0.1f;
	}

	// camera control
	float deltaTime = mTimer.GetElapsedTime();
	const float boostModifyer = 1.6;
	const float cameraMoveSpeed = iS->IsKeyDown(Keys::LSHIFT) ? 10.f : 10.f*boostModifyer;
	const float cameraTurnSpeed = 0.7f;
	if(iS->IsKeyDown(Keys::W))
	{
		mCameraTransform.Walk(cameraMoveSpeed * deltaTime);
	}
	if(iS->IsKeyDown(Keys::S))
	{
		mCameraTransform.Walk(-cameraMoveSpeed * deltaTime);
	}
	if(iS->IsKeyDown(Keys::A))
	{
		mCameraTransform.Strafe(-cameraMoveSpeed * deltaTime);
	}
	if(iS->IsKeyDown(Keys::D))
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

	Math::Matrix4 matRotX = Math::Matrix4::RotationX(mTimer.GetTotalTime() * 3.0f);
	Math::Matrix4 matRotY = Math::Matrix4::RotationY(mTimer.GetTotalTime() * 2.5f);
	Math::Matrix4 matRotZ = Math::Matrix4::RotationZ(mTimer.GetTotalTime() * 2.0f);
	Math::Matrix4 matTrans = Math::Matrix4::Translation(0.f, 5.f, 0.f);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(1.f, 1.f, 1.f);

	Math::Matrix4 worldMatrix = matRotX * matRotY * matRotZ * matTrans * matScale;
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
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

	Graphics::SimpleDraw::DrawSphere(Math::Vector3(10.f,0.f,10.f), numSlices, numRings, scale, Math::Vector4::Cyan());

	Graphics::SimpleDraw::Flush(viewMatrix*projectionMatrix);

	gs->EndRender();
}