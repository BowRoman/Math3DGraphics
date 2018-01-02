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

	void GenerateDepthMap();
	void DrawScene();

	//struct ConstantData
	//{
	//	Math::Matrix4 wvp;
	//	Math::Matrix4 world;
	//	Math::Vector3 cameraPosition;
	//	float padding;
	//	Graphics::Light light;
	//	Graphics::Material material;
	//	float displacmentScale;
	//	float padding2;
	//	float padding3;
	//	float padding4;
	//	Math::Matrix4 view;
	//	Math::Matrix4 projection;
	//};

	struct ConstantDataDepth
	{
		Math::Matrix4 wvp;
		float displacmentScale;
	};


	struct ConstantShadowMapData
	{
		Math::Matrix4 World;
		Math::Matrix4 WVP;
		Math::Matrix4 WVPLight;
		Math::Vector4 ViewPosition;
		Math::Vector4 LightDirection;
		Math::Vector4 LightAmbient;
		Math::Vector4 LightDiffuse;
		Math::Vector4 LightSpecular;
		Math::Vector4 MaterialAmbient;
		Math::Vector4 MaterialDiffuse;
		Math::Vector4 MaterialSpecular;
		float MaterialPower;
		float DisplacementScale;
	};


	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	//Graphics::Transform mCameraTransform;

	Graphics::Camera mLightCamera;
	//Graphics::Transform mLightCameraTransform;

	Graphics::Camera* mCurrentCamera;
	//Graphics::Transform* mCurrentCameraTransform;

	Graphics::Light mLight;
	Graphics::Material mMaterial;



	//Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;

	Graphics::MeshBuffer mMeshBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

	Graphics::VertexShader mVertexPostProcessShader;
	Graphics::PixelShader mPixelPostProcessShader;

	//earth mesh
	Graphics::Mesh mMesh;
	Graphics::Texture mTexture;
	Graphics::Texture mSpecMap;
	Graphics::Texture mNormalMap;
	Graphics::Texture mDisplacementMap;
	Graphics::Sampler mSampler;

	//skybox
	Graphics::Texture mSkyBoxTexture;
	Graphics::VertexShader mSkyBoxVertexShader;
	Graphics::PixelShader mSkyBoxPixelShader;
	Graphics::MeshBuffer mSkyBoxMeshBuffer;


	//Plane mesh
	Graphics::Mesh mPlane;
	Graphics::MeshBuffer mPlaneMeshBuffer;
	Graphics::VertexShader mPlaneVertexShader;
	Graphics::PixelShader mPlanePixelShader;
	Graphics::Texture mPlaneTexture;
	Graphics::Texture mPlaneSpecTexture;
	Graphics::Texture mPlaneNormalTexture;
	Graphics::Texture mPlaneDisplacementTexture;


	//render target
	Graphics::RenderTarget mRenderTarget;
	Graphics::RenderTargetMesh mRenderTargetMeshBuffer;


	//depth map
	Graphics::DepthMap mDepthMap;
	Graphics::PixelShader mDepthMapPixelShader;
	Graphics::VertexShader mDepthMapVertexShader;
	Graphics::TypedConstantBuffer<ConstantDataDepth> mDepthMapConstantBuffer;
	Graphics::Sampler mLinearSampler;

	//shadow map
	//Graphics::PixelShader mShadowPixelShader;
	//Graphics::VertexShader mShadowVertexShader;
	Graphics::TypedConstantBuffer<ConstantShadowMapData> mShadowConstantBuffer;

	float mRotation;
};
