#pragma once
#include <Core\Inc\Core.h>
#include <Input\Inc\Input.h>
#include <Graphics\Inc\Graphics.h>

class GameApp : public Core::Application
{
public:
	GameApp();
	~GameApp() override;
private:
	void OnInitialize(uint32_t width, uint32_t height) override;
	void OnTerminate() override;
	void OnUpdate() override;

	Core::Window mWindow;

	Graphics::MeshBuffer mMeshBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

};
