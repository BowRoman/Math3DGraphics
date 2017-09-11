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


	struct ConstantData
	{
		Math::Matrix4 wvp;
	};
	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	Graphics::Transform mCameraTransform;

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::MeshBuffer mTerrainMeshBuffer;
	Graphics::MeshBuffer mSphereMeshBuffer;

	Graphics::VertexShader mTerrainVertexShader;
	Graphics::VertexShader mSphereVertexShader;

	Graphics::PixelShader mTerrainPixelShader;
	Graphics::PixelShader mSpherePixelShader;

	Graphics::Texture mTerrainTexture;
	Graphics::Texture mSphereTexture;

	Graphics::Sampler mTerrainSampler;
	Graphics::Sampler mSphereSampler;

	Graphics::Mesh mMesh;
	Graphics::MeshBuilder mMeshBuilder;
	Graphics::HeightMap mHeightMap;

	Math::Vector3 mRotation;
};
