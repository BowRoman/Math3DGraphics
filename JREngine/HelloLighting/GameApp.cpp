#include "GameApp.h"
#include <assert.h>

namespace
{
	//Graphics::VertexPT* Vertices;
	//uint32_t VertexCount;

	//uint32_t* Indices;
	//uint32_t IndexCount;


	uint32_t stacks = 20;
	uint32_t slices = 20;
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
	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::format);
	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	
	mTexture.Initialize(L"../Assets/Images/earth.jpg");
	mSampler.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp);

	mMeshBuilder.CreateSphere(mMesh, slices, stacks);

	mMeshBuffer.Initialize(mMesh.GetVertices(), sizeof(Graphics::Vertex), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());
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

	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f, 0.0f, -50.0f + i);
		Math::Vector3 p1(+50.0f, 0.0f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::White());
	}
	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, 0.0f, -50.0f);
		Math::Vector3 p1(-50.0f + i, 0.0f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::White());
	}

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}