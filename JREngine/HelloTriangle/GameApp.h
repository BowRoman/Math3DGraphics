#pragma once
#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core\Inc\Core.h>
#include <Graphics\Inc\Graphics.h>
#include <Input\Inc\Input.h>

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
	Core::Window mWindow;
	Graphics::MeshBuffer mMesh;
	Graphics::PixelShader mPixelShader;
	Graphics::VertexShader mVertexShader;
};

#endif // #ifndef INCLUDED_GAMEAPP_H