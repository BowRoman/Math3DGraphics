#include "GameApp.h"
#include <assert.h>

// http://www.chadvernon.com/blog/resources/directx9/terrain-generation-with-a-heightmap/

namespace
{

	const Graphics::VertexPT KVertices[] =
	{
		{ Math::Vector3(-0.5f,-0.5f,0.0f),	Math::Vector2(0.0f, 1.0f) }, //0
		{ Math::Vector3(-0.5f,0.5f,0.0f),	Math::Vector2(0.0f, 0.0f) }, //1
		{ Math::Vector3(0.5f,0.5f,0.0f),	Math::Vector2(1.0f, 0.0f) }, //2
		{ Math::Vector3(0.5f,-0.5f,0.0f),	Math::Vector2(1.0f, 1.0f) }, //3


		//{ Math::Vector3(-0.5f,-0.5f,0.0f),	Math::Vector2(1.0f, 0.0f) }, //0
		//{ Math::Vector3(-0.5f,0.5f,0.0f),	Math::Vector2(1.0f, 1.0f) }, //1
		//{ Math::Vector3(0.5f,0.5f,0.0f),	Math::Vector2(0.0f, 1.0f) }, //2
		//{ Math::Vector3(0.5f,-0.5f,0.0f),	Math::Vector2(0.0f, 0.0f) }, //3
	};
	const int kVertexCount = sizeof(KVertices) / sizeof(KVertices[0]);

	const uint32_t kIndices[] =
	{
		0,1,2,
		2,3,0
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

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 2.0f, -5.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));

	mConstantBuffer.Initialize();
	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::VertexPT::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mTexture.Initialize(L"../Assets/Images/tree.dds");
	//mTexture.Initialize(L"../Assets/Images/brickwork.jpg");
	mSampler.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp);

	mMeshBuffer.Initialize(KVertices, sizeof(Graphics::VertexPT), kVertexCount, kIndices, kIndexCount);

}


void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
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

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());


	/*Math::Matrix4x4 rotateZ = Math::Matrix4x4::RotateZ(mTimer.GetTotalTime() * 1.0f);
	worldMatrix *= rotateZ;*/
	ConstantData data;
	data.wvp = Math::Transpose( viewMatrix * projectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS(); //matrix to vertex shader


	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindVS(0);
	mTexture.BindPS(0);

	mMeshBuffer.Render();

	for (int i = 0; i < 1000; ++i)
	{
		Math::Vector3 p0(-500.0f, 0.0f, -500.0f + i);
		Math::Vector3 p1(+500.0f, 0.0f, -500.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Cyan());
	}
	for (int i = 0; i < 1000; ++i)
	{
		Math::Vector3 p0(-500.0f + i, 0.0f, -500.0f);
		Math::Vector3 p1(-500.0f + i, 0.0f, +500.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Orange());
	}

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}