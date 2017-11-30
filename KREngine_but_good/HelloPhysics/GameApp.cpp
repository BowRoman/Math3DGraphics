#include "GameApp.h"
#include <assert.h>



namespace
{
	
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

}


void GameApp::OnTerminate()
{
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
	float dTime = mTimer.GetElapsedTime();
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
	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * dTime);
		mCameraTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::ONE))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 100; ++i)
		{
			auto p = new Physics::Particle();
			p->SetPosition({ 0.0f,0.0f,0.0f });
			p->SetVelocity({
				Math::Random::GetF(-0.1f,  +0.1f),
				Math::Random::GetF(+0.05f, +0.15f),
				Math::Random::GetF(-0.1f,  +0.1f)
			});
			mPhysicsWorld.AddParticle(p);
		}
	}
	if (is->IsKeyDown(Keys::TWO))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 50; ++i)
		{
			auto p0 = new Physics::Particle();
			p0->SetPosition({ 0.0f,0.0f,0.0f });
			p0->SetRadius(0.5f);
			p0->SetVelocity({
				Math::Random::GetF(-0.1f,  +0.1f),
				Math::Random::GetF(+0.05f, +0.15f),
				Math::Random::GetF(-0.1f,  +0.1f)
			});
			auto p1 = new Physics::Particle();
			p1->SetPosition({ 0.0f,0.0f,0.0f });
			p1->SetRadius(0.5f);
			p1->SetVelocity({
				Math::Random::GetF(-0.1f,  +0.1f),
				Math::Random::GetF(+0.05f, +0.15f),
				Math::Random::GetF(-0.1f,  +0.1f)
			});
			mPhysicsWorld.AddParticle(p0);
			mPhysicsWorld.AddParticle(p1);
			auto c0 = new Physics::Spring(p0,p1,1.0f);
			mPhysicsWorld.AddConstraint(c0);
		}
	}
	mPhysicsWorld.Update(dTime);

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());


	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f, -0.1f, -50.0f + i);
		Math::Vector3 p1(+50.0f, -0.1f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Black());
	}
	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, -0.1f, -50.0f);
		Math::Vector3 p1(-50.0f + i, -0.1f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Black());
	}

	Graphics::SimpleDraw::DrawTransform(Math::Matrix4::Identity());
	mPhysicsWorld.DebugDraw();

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}