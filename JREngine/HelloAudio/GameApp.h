#pragma once
#include <AudioDirectX\Inc\Audio.h>
#include <Core\Inc\Core.h>
#include <Input\Inc\Input.h>
#include <Graphics\Inc\Graphics.h>
#include <Physics\Inc\Physics.h>

class GameApp : public Core::Application
{
public:
	GameApp();
	~GameApp() override;
private:
	void OnInitialize(uint32_t width, uint32_t height) override;
	void OnTerminate() override;
	void OnUpdate() override;

	struct ConstantData
	{
		Math::Matrix4 wvp;
	};
	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	Graphics::Transform mCameraTransform;

	Physics::PhysicsWorld mPhysicsWorld;
};
