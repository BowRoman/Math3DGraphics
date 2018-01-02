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

	struct ConstantData
	{
		Math::Matrix4 wvp;
	};

	struct ConstantDepthData
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
	Graphics::Camera* mCurrentCamera;

	// light
	Graphics::Camera mLightCamera;
	Graphics::Light mLight;
	Graphics::Material mMaterial;

	//depth map
	Graphics::DepthMap mDepthMap;
	Graphics::PixelShader mDepthMapPixelShader;
	Graphics::VertexShader mDepthMapVertexShader;
	Graphics::TypedConstantBuffer<ConstantDepthData> mDepthMapConstantBuffer;
	Graphics::Sampler mLinearSampler;

	// shadow
	Graphics::TypedConstantBuffer<ConstantShadowMapData> mShadowConstantBuffer;

	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

	Graphics::Sampler mSampler;

	// duck model
	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::Model mModel;

	// Plane mesh
	Graphics::Mesh mPlane;
	Graphics::MeshBuffer mPlaneMeshBuffer;
	Graphics::VertexShader mPlaneVertexShader;
	Graphics::PixelShader mPlanePixelShader;
	Graphics::Texture mPlaneTexture;
	Graphics::Texture mPlaneSpecTexture;
	Graphics::Texture mPlaneNormalTexture;
	Graphics::Texture mPlaneDisplacementTexture;

	float mRotation;
};
