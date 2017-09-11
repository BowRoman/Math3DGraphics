#include "GameApp.h"
#include <assert.h>

namespace
{
	//Graphics::VertexPT* Vertices;
	//uint32_t VertexCount;

	//uint32_t* Indices;
	//uint32_t IndexCount;


	uint32_t stacks = 1024;
	uint32_t slices = 768;
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
	Graphics::TextureManager::StaticInitialize("../Assets/Images/");

	mTimer.Initialize();

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 2.0f, -5.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));


	mConstantBuffer.Initialize();
	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	
	mTexture.Initialize(L"../Assets/Images/duck.dds");
	mSampler.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp);

	mModel.Load("../Assets/Models/duck.txt");
	//mMeshBuilder.GenerateStackSliceSphere(mMesh,stacks, slices, 1.0f);

	//mMeshBuffer.Initialize(mMesh.mVertices, sizeof(Graphics::Vertex), mMesh.mVertexCount, mMesh.mIndices, mMesh.mIndexCount);

}


void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	//mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	mModel.Unload();
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
	const float cameraMoveSpeed = 10.0f;
	const float cameraTurnSpeed = 1.0f;
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

	Math::Matrix4 worldMatrix = Math::Matrix4::Scaling(0.1);
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());


	/*Math::Matrix4x4 rotateZ = Math::Matrix4x4::RotateZ(mTimer.GetTotalTime() * 1.0f);
	worldMatrix *= rotateZ;*/
	ConstantData data;
	data.wvp = Math::Transpose( worldMatrix * viewMatrix * projectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS(); //matrix to vertex shader


	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindVS(0);
	mTexture.BindPS(0);

	mModel.Render();
	//mMeshBuffer.Render();

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