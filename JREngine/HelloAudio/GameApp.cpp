#include "GameApp.h"
#include <assert.h>



namespace
{
	std::vector<Audio::SoundId> soundIds;
	std::vector<Audio::SoundId> instIds;
	bool songPlaying = false;
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

	Audio::AudioSystem::StaticInitialize();
	Audio::SoundManager::StaticInitialize();
	auto soundMng = Audio::SoundManager::Get();
	soundMng->SetFilePath("..\\Assets\\Sounds");
	soundIds.push_back(soundMng->Load("I_Was_Enjoying_That!.wav"));
	soundIds.push_back(soundMng->Load("LightningCombat_1.wav"));
	instIds.push_back(soundMng->CreateInstance(soundIds[1]));
}


void GameApp::OnTerminate()
{
	//Terminate
	mPhysicsWorld.ClearDynamic();

	Audio::SoundManager::Get()->Clear();
	Audio::SoundManager::StaticTerminate();
	Audio::AudioSystem::StaticTerminate();

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

	Input::InputSystem* is = Input::InputSystem::Get();
	is->Update();
	if (is->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}
	const float cameraBaseMoveSpeed = 10.0f;
	float cameraMoveSpeed = 10.0f;
	const float cameraTurnSpeed = 1.0f;
	float dTime = mTimer.GetElapsedTime();
	if (is->IsKeyDown(Keys::LSHIFT))
	{
		cameraMoveSpeed = cameraBaseMoveSpeed * 2.5f;
	}
	else
	{
		cameraMoveSpeed = cameraBaseMoveSpeed;
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
	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * dTime);
		mCameraTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * dTime);
	}
	if (is->IsKeyPressed(Keys::P))
	{
		if (songPlaying)
		{
			Audio::SoundManager::Get()->Pause(instIds[0]);
		}
		else
		{
			Audio::SoundManager::Get()->Play(instIds[0]);
		}
		songPlaying = !songPlaying;
	}
	if (is->IsKeyPressed(Keys::S))
	{
		Audio::SoundManager::Get()->Stop(instIds[0]);
		songPlaying = false;
	}
	if (is->IsKeyPressed(Keys::ONE))
	{
		Audio::SoundManager::Get()->PlayEffect(soundIds[0]);
	}
	// Fixed
	if (is->IsKeyPressed(Keys::THREE))
	{
		for (int i = 0; i < 50; ++i)
		{
			auto p = new Physics::Particle();
			p->SetPosition({ 0.0f,0.0f,0.0f });
			mPhysicsWorld.AddParticle(p);
			auto c = new Physics::Fixed(p, Math::Vector3(Math::Random::GetF(-5.0f, 5.0f), Math::Random::GetF(-5.0f, 5.0f), Math::Random::GetF(-5.0f, 5.0f)));
			mPhysicsWorld.AddConstraint(c);
		}
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	Audio::AudioSystem::Get()->Update();

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