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

	// TODO: Replace with class Graphics::Mesh
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	// TODO: Replace with class Graphics::VertexShader
	ID3D11InputLayout* mInputLayout;
	ID3D11VertexShader* mVertexShader;

	// TODO: Replace with class Graphics::PixelShader
	ID3D11PixelShader* mPixelShader;
};

#endif // #ifndef INCLUDED_GAMEAPP_H

namespace Graphics
{
	class Mesh
	{
		// con/destructor
		void Initialize(...);
		void Terminate();
		void Render();
	};
	class VertexShader//PixelShader
	{
		// con/destructor
		void Initialize(...);
		void Terminate();
		void Bind();
	}
}