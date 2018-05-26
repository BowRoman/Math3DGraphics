#pragma once
#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core\Inc\Core.h>
#include <Graphics\Inc\Graphics.h>
#include <Input\Inc\Input.h>
#include <GameEngine\Inc\GameEngine.h>

class GameApp : public Core::Application
{
public:
	GameApp();
	~GameApp() override;
private:
	void OnInitialize(uint32_t width, uint32_t height) override;
	void OnTerminate() override;
	void OnUpdate() override;

private:
	struct ConstantData
	{
		Math::Matrix4 wvp;
	};

	Core::Window mWindow;
	Core::Timer mTimer;

	GameEngine::World mWorld;
};

#endif // #ifndef INCLUDED_GAMEAPP_H