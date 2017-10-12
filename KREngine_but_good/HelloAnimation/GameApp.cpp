#include "GameApp.h"
#include <assert.h>



namespace
{
	
	const Graphics::VertexPC KVertices[] =
	{
		{ Math::Vector3(-0.9f,	 0.4f,	 0.0f),	Math::Vector4::Cyan() }, //0
		{ Math::Vector3(-0.45f,	 0.9f,	 0.0f),	Math::Vector4::Cyan() }, //1
		{ Math::Vector3(0.0f,	 0.4f,	 0.0f),	Math::Vector4::Cyan() }, //2
		{ Math::Vector3(0.45f,	 0.9f,	 0.0f),	Math::Vector4::Cyan() }, //3
		{ Math::Vector3(0.9f,	 0.4f,	 0.0f),	Math::Vector4::Cyan() }, //4
		{ Math::Vector3(0.9f,	-0.2f,	 0.0f),	Math::Vector4::Cyan() }, //5
		{ Math::Vector3(-0.9f,	-0.2f,	 0.0f),	Math::Vector4::Blue() }, //6
		{ Math::Vector3(0.0f,	-0.9f,	 0.0f),	Math::Vector4::Blue() }, //7

	};
	const int kVertexCount = sizeof(KVertices) / sizeof(KVertices[0]);

	const uint32_t kIndices[] =
	{
		//front
		0,1,2,
		2,3,4,
		0,4,5,
		0,5,6,
		6,5,7,

		//back
		2,1,0,
		4,3,2,
		5,4,0,
		6,5,0,
		7,5,6
	};
	const int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);
}

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

void GameApp::OnInitialize(uint32_t width, uint32_t height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::SimpleDraw::StaticInitialize(10000);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mTimer.Initialize();

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 10.0f, -10.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));

	mConstantBuffer.Initialize();
	mVertexShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx");
	mMeshBuffer.Initialize(KVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);

	mAnimation.Initialize(true, 4);
	mAnimation.AddKeyframe(0.0f, Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(1.0f, 1.0f, 1.0f), Math::Quaternion::Identity());
	mAnimation.AddKeyframe(0.01f, Math::Vector3(6.0f, 4.0f, 2.0f), Math::Vector3(1.0f, 1.0f, 1.0f), Math::Quaternion::RotationAxis(Math::Vector3::YAxis(), 4.0f));
	mAnimation.AddKeyframe(0.02f, Math::Vector3(-2.0f, -2.0f, -2.0f), Math::Vector3(3.0f, 3.0f, 3.0f), Math::Quaternion::RotationAxis(Math::Vector3::XAxis(), 4.0f));
	mAnimation.AddKeyframe(0.03f, Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(1.0f, 1.0f, 1.0f), Math::Quaternion::Identity());
}


void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mAnimation.Terminate();
	//Terminate

	Input::InputSystem::StaticTerminate();
	Graphics::SimpleDraw::StaticTerminate();
	Graphics::GraphicsSystem::StaticTerminate();

	UnHookWindow();
	mWindow.Terminate();
}


void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
	}
	mTimer.Update();

	Input::InputSystem* is = Input::InputSystem::Get();
	is->Update();
	if (is->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}
	const float baseCameraMoveSpeed = 10.0f;
	float cameraMoveSpeed = baseCameraMoveSpeed;
	const float cameraTurnSpeed = 1.0f;
	if (is->IsKeyDown(Keys::LSHIFT))
	{
		cameraMoveSpeed = baseCameraMoveSpeed * 2.0;
	}
	if (is->IsKeyDown(Keys::W))
	{
		mCameraTransform.Walk(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::S))
	{
		mCameraTransform.Walk(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::D))
	{
		mCameraTransform.Strafe(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::A))
	{
		mCameraTransform.Strafe(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::E))
	{
		mCameraTransform.Rise(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::Q))
	{
		mCameraTransform.Rise(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}

	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * mTimer.GetElapsedTime());
		mCameraTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * mTimer.GetElapsedTime());
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = /*Math::Matrix4::Scaling(5.0f) **/ mAnimation.GetTransform(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	/*Math::Matrix4x4 rotateZ = Math::Matrix4x4::RotateZ(mTimer.GetTotalTime() * 1.0f);
	worldMatrix *= rotateZ;*/
	ConstantData data;
	data.wvp = Math::Transpose(worldMatrix * viewMatrix * projectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS(); //matrix to vertex shader

	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();

	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f, 0.0f, -50.0f + i);
		Math::Vector3 p1(+50.0f, 0.0f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Cyan());
	}
	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, 0.0f, -50.0f);
		Math::Vector3 p1(-50.0f + i, 0.0f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Orange());
	}

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}