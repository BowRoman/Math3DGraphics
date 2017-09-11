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

	//TODO: Add to graphics library		Light.h
	struct Light
	{
		Math::Vector3 direction;
		float padding;
		Math::Vector4 ambient;
		Math::Vector4 diffuse;
		Math::Vector4 specular;

		Light()
			: direction(Math::Vector3::ZAxis())
			, ambient(Math::Vector4::White())
			, diffuse(Math::Vector4::White())
			, specular(Math::Vector4::White()) 
		{}
	};

	//TODO: Add to graphics library		Material.h
	struct Material
	{
		Math::Vector4 ambient;
		Math::Vector4 diffuse;
		Math::Vector4 specular;
		float specularPower;

		Material()
			:ambient(Math::Vector4::White())
			, diffuse(Math::Vector4::White())
			, specular(Math::Vector4::White())
			, specularPower(1.0f)
		{}

	};



	struct ConstantData
	{
		Math::Matrix4 wvp;
		Math::Matrix4 world;
		Math::Vector3 cameraPosition;
		float padding;
		Light light;
		Material material;
		float displacmentScale;
	};



	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	Graphics::Transform mCameraTransform;

	Light mLight;
	Material mMaterial;



	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::MeshBuffer mMeshBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

	Graphics::VertexShader mSkyBoxVertexShader;
	Graphics::PixelShader mSkyBoxPixelShader;

	Graphics::Texture mTexture;

	Graphics::Texture mSpecMap;
	Graphics::Texture mNormalMap;
	Graphics::Texture mDisplacementMap;
	Graphics::Sampler mSampler;

	Graphics::Texture mSkyBox;

	Graphics::Mesh mMesh;
	Graphics::MeshBuilder mMeshBuilder;

	Math::Vector3 mRotation;
};
