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
		Math::Matrix4 world;
		Math::Vector3 cameraPosition;
		float padding;
		Graphics::Light light;
		Graphics::Material material;
		float displacmentScale;
		float padding2;
		float padding3;
		float padding4;
		Math::Matrix4 view;
		Math::Matrix4 projection;
	};



	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	Graphics::Transform mCameraTransform;

	Graphics::Light mLight;
	Graphics::Material mMaterial;



	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::MeshBuffer mMeshBuffer;
	Graphics::RenderTargetMesh mRenderTargetMeshBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

	Graphics::VertexShader mVertexPostProcessShader;
	Graphics::PixelShader mPixelPostProcessShader;

	Graphics::Texture mTexture;

	Graphics::Texture mSpecMap;
	Graphics::Texture mNormalMap;
	Graphics::Texture mDisplacementMap;
	Graphics::Sampler mSampler;

	Graphics::Texture mSkyBoxTexture;
	Graphics::VertexShader mSkyBoxVertexShader;
	Graphics::PixelShader mSkyBoxPixelShader;
	Graphics::MeshBuffer mSkyBoxMeshBuffer;

	Graphics::Mesh mMesh;
	Graphics::MeshBuilder mMeshBuilder;

	Graphics::RenderTarget mRenderTarget;


	Math::Vector3 mRotation;
};
