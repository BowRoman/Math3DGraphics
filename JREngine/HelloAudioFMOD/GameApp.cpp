#include "GameApp.h"
#include <assert.h>



namespace
{
	std::vector<uint32_t> soundIds;
	std::vector<uint32_t> channelIds;
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

	// Particles
	Physics::Settings settings;
	settings.gravity = Math::Vector3::Zero();
	mPhysicsWorld.Setup(settings);
	for (int i = 0; i < 51; ++i)
	{
		auto p = new Physics::Particle();
		p->SetPosition({ -12.5f + (static_cast<float>(i)*0.5f), 0.0f, 0.0f });
		mPhysicsWorld.AddParticle(p);
	}

	// Audio
	Audio::JRAudioEngine::StaticInitialize();
	auto soundMng = Audio::JRAudioEngine::Get();
	soundMng->SetRoot("..\\Assets\\Sounds");
	soundIds.push_back(soundMng->LoadSound("I_Was_Enjoying_That!.wav"));
	//soundIds.push_back(soundMng->("LightningCombat_1.wav"));
	//instIds.push_back(soundMng->CreateInstance(soundIds[1]));
}


void GameApp::OnTerminate()
{
	//Terminate
	mPhysicsWorld.ClearDynamic();

	Audio::JRAudioEngine::StaticTerminate();

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
			//Audio::JRAudioEngine::Get()->Pause(instIds[0]);
		}
		else
		{
			//Audio::JRAudioEngine::Get()->Play(instIds[0]);
		}
		songPlaying = !songPlaying;
	}
	if (is->IsKeyPressed(Keys::O))
	{
		//Audio::SoundManager::Get()->Stop(instIds[0]);
		songPlaying = false;
	}
	if (is->IsKeyPressed(Keys::ONE))
	{
		Audio::JRAudioEngine::Get()->Play(soundIds[0]);
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	Audio::JRAudioEngine::Get()->Update();

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