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
	Graphics::SimpleDraw::StaticInitialize(100000);
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
	if (is->IsKeyPressed(Keys::ONE))
	{
		Math::Plane plane{0.0f,1.0f,0.0f,-0.1f };
		for (int i = 0; i < 100; ++i)
		{
			auto p = new Physics::Particle();
			p->SetPosition({ 0.0f,3.0f,0.0f });
			p->SetVelocity({
				Math::Random::GetF(-0.1f,  +0.1f),
				Math::Random::GetF(+0.05f, +0.15f),
				Math::Random::GetF(-0.1f,  +0.1f)
			});
			mPhysicsWorld.AddParticle(p);
			auto c = new Physics::Wall(p, plane);
			mPhysicsWorld.AddConstraint(c);
		}
	}
	if (is->IsKeyPressed(Keys::TWO))
	{
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
			auto c0 = new Physics::Spring(p0, p1, 1.0f);
			mPhysicsWorld.AddConstraint(c0);
		}
	}
	if (is->IsKeyPressed(Keys::THREE))
	{
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
			auto c = new Physics::Fixed(p, Math::Vector3(Math::Random::GetF(-5.0f, 5.0f), Math::Random::GetF(-5.0f, 5.0f), Math::Random::GetF(-5.0f, 5.0f)));
			mPhysicsWorld.AddConstraint(c);
		}
	}
	if (is->IsKeyPressed(Keys::FOUR))
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
		auto p2 = new Physics::Particle();
		p2->SetPosition({ 0.0f,0.0f,0.0f });
		p2->SetRadius(0.5f);
		p2->SetVelocity({
			Math::Random::GetF(-0.1f,  +0.1f),
			Math::Random::GetF(+0.05f, +0.15f),
			Math::Random::GetF(-0.1f,  +0.1f)
		});
		auto p3 = new Physics::Particle();
		p3->SetPosition({ 0.0f,0.0f,0.0f });
		p3->SetRadius(0.5f);
		p3->SetVelocity({
			Math::Random::GetF(-0.1f,  +0.1f),
			Math::Random::GetF(+0.05f, +0.15f),
			Math::Random::GetF(-0.1f,  +0.1f)
		});
		auto p4 = new Physics::Particle();
		p4->SetPosition({ 0.0f,0.0f,0.0f });
		p4->SetRadius(0.5f);
		p4->SetVelocity({
			Math::Random::GetF(-0.1f,  +0.1f),
			Math::Random::GetF(+0.05f, +0.15f),
			Math::Random::GetF(-0.1f,  +0.1f)
		});
		mPhysicsWorld.AddParticle(p0);
		mPhysicsWorld.AddParticle(p1);
		mPhysicsWorld.AddParticle(p2);
		mPhysicsWorld.AddParticle(p3);
		mPhysicsWorld.AddParticle(p4);
		auto c0 = new Physics::Spring(p0, p1, 1.0f);
		auto c1 = new Physics::Spring(p1, p2, 1.0f);
		auto c2 = new Physics::Spring(p2, p3, 1.0f);
		auto c3 = new Physics::Spring(p3, p4, 1.0f);
		mPhysicsWorld.AddConstraint(c0);
		mPhysicsWorld.AddConstraint(c1);
		mPhysicsWorld.AddConstraint(c2);
		mPhysicsWorld.AddConstraint(c3);
		auto c4 = new Physics::Fixed(p0, Math::Vector3(Math::Random::GetF(-5.0f, 5.0f), Math::Random::GetF(-2.0f, 2.0f), Math::Random::GetF(-5.0f, 5.0f)));
		auto c5 = new Physics::Fixed(p4, Math::Vector3(Math::Random::GetF(-5.0f, 5.0f), Math::Random::GetF(-2.0f, 2.0f), Math::Random::GetF(-5.0f, 5.0f)));
		mPhysicsWorld.AddConstraint(c4);
		mPhysicsWorld.AddConstraint(c5);
	}
	if (is->IsKeyPressed(Keys::FIVE))
	{
		mPhysicsWorld.AddCube(mPhysicsWorld, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f }, 1.0f, 10.0f, false);
	}
	if (is->IsKeyPressed(Keys::SIX))
	{
		std::vector<Physics::Particle*> leftParticles;
		std::vector<Physics::Particle*> rightParticles;
		auto p0 = new Physics::Particle();
		auto p1 = new Physics::Particle();
		p0->SetPosition({ 0.0f,10.0f,-0.5f });
		p1->SetPosition({ 0.0f,10.0f,+0.5f });
		leftParticles.push_back(p0);
		rightParticles.push_back(p1);
		mPhysicsWorld.AddParticle(p0);
		mPhysicsWorld.AddParticle(p1);
		auto c = new Physics::Spring(p0, p1, 1.0f);
		mPhysicsWorld.AddConstraint(c);
		for (int i = 0; i < 10; ++i)
		{
			p0 = new Physics::Particle();
			p0->SetPosition({ (i + 1.0f)*0.75f,(i + 10.0f)*0.75f, -0.5f });

			p1 = new Physics::Particle();
			p1->SetPosition({ (i + 1.0f)*0.75f,(i + 10.0f)*0.75f, +0.5f });

			leftParticles.push_back(p0);
			rightParticles.push_back(p1);

			mPhysicsWorld.AddParticle(p0);
			mPhysicsWorld.AddParticle(p1);

			auto c0 = new Physics::Spring(p0, p1, 1.0f);
			auto c1 = new Physics::Spring(p0, leftParticles[i], 1.0f);
			auto c2 = new Physics::Spring(p1, rightParticles[i], 1.0f);
			mPhysicsWorld.AddConstraint(c0);
			mPhysicsWorld.AddConstraint(c1);
			mPhysicsWorld.AddConstraint(c2);
		}
		auto f0 = new Physics::Fixed(leftParticles[0], { 0.0f,10.0f,-0.5f });
		auto f1 = new Physics::Fixed(rightParticles[0], { 0.0f,10.0f,+0.5f });
		mPhysicsWorld.AddConstraint(f0);
		mPhysicsWorld.AddConstraint(f1);
	}
	if (is->IsKeyPressed(Keys::GRAVE))
	{
		mPhysicsWorld.ClearDynamic();
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());


	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f, -0.1f, -50.0f + i);
		Math::Vector3 p1(+50.0f, -0.1f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Gray());
	}
	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, -0.1f, -50.0f);
		Math::Vector3 p1(-50.0f + i, -0.1f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Gray());
	}

	Graphics::SimpleDraw::DrawTransform(Math::Matrix4::Identity());
	mPhysicsWorld.DebugDraw();
	mPhysicsWorld.Update(dTime);

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}