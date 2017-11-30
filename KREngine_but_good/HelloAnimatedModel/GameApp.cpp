#include "GameApp.h"
#include <assert.h>

// TODO: Add propagating bone transforms and visualize skeleton.

namespace
{
	// world matrix
	Math::Matrix4 worldMatrix;
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
	Graphics::SimpleDraw::StaticInitialize(100000);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::TextureManager::StaticInitialize("../Assets/Images/");

	mTimer.Initialize();

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 10.0f, -10.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));


	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");
	mModel.Load("../Assets/Models/snorlax.txt");
	bPlay = false;
	mConstantBuffer.Initialize();

}


void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mConstantBuffer.Terminate();
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
	const float baseCameraMoveSpeed = 10.0f;
	float cameraMoveSpeed = baseCameraMoveSpeed;
	const float cameraTurnSpeed = 1.0f;
	float dTime = mTimer.GetElapsedTime();
	if (is->IsKeyDown(Keys::LSHIFT))
	{
		cameraMoveSpeed = baseCameraMoveSpeed * 2.0;
	}
	if (is->IsKeyDown(Keys::W))
	{
		mCameraTransform.Walk(cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::S))
	{
		mCameraTransform.Walk(-cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::D))
	{
		mCameraTransform.Strafe(cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::A))
	{
		mCameraTransform.Strafe(-cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::E))
	{
		mCameraTransform.Rise(cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::Q))
	{
		mCameraTransform.Rise(-cameraMoveSpeed * dTime);
	}
	if (is->IsKeyPressed(Keys::P))
	{
		bPlay = !bPlay;
		if (bPlay)
			mModel.Play();
		else
			mModel.Pause();
	}

	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * dTime);
		mCameraTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * dTime);
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	worldMatrix = Math::Matrix4::Scaling(5.0f) * Math::Matrix4::RotationY(Math::kPi);
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	mVertexShader.Bind();
	mPixelShader.Bind();

	ConstantData data;
	data.wvp = Math::Transpose(worldMatrix * viewMatrix * projectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mModel.Update(dTime);
	//mModel.Render();
	Graphics::Bone* rootBone;
	rootBone = mModel.GetRoot();
	std::vector<Math::Matrix4> matVec;
	DrawBones(rootBone, Math::Matrix4::Identity());

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
void GameApp::DrawBones(Graphics::Bone* bone, Math::Matrix4 parentTransform)
{
	Math::Matrix4 transform = mModel.GetBoneTransform(bone->index);
	Math::Vector3 pos1 = Math::Vector3(transform._41, transform._42, transform._43);
	Graphics::SimpleDraw::DrawSphere(pos1, 5, 5, 0.05f);
	if (bone->parent)
	{
		parentTransform = mModel.GetBoneTransform(bone->parentIndex);

		pos1 = Math::Vector3(transform._41, transform._42, transform._43);
		Math::Vector3 pos2 = Math::Vector3(parentTransform._41, parentTransform._42, parentTransform._43);
		Math::Vector4 color = Math::Vector4::CyanGreen();
		Graphics::SimpleDraw::DrawLine(pos1, pos2, color);
	}
	for (int i = 0; i < bone->children.size(); ++i)
	{
		DrawBones(bone->children[i], transform);
	}
}
