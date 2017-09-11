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
	struct ConstantData
	{
		Math::Matrix4 wvp;
	};

	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	Graphics::Transform mCameraTransform;

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;

	Graphics::Terrain mTerrain;
	Graphics::Texture mTerrainTexture;
	Graphics::Sampler mTerrainSampler;
	Graphics::PixelShader mTerrainPixelShader;
	Graphics::VertexShader mTerrainVertexShader;

	Math::Vector3 mRotation;
};

#endif // #ifndef INCLUDED_GAMEAPP_H