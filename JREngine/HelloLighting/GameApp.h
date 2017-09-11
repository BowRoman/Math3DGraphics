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

	struct Light // TODO: Add to Graphics library (i.e. Graphics::Light)
	{
		Light()
			: direction(Math::Vector3::ZAxis(), 0.0f)
			, ambient(Math::Vector4::White())
			, diffuse(Math::Vector4::White())
			, specular(Math::Vector4::White())
		{}

		Math::Vector4 direction;
		Math::Vector4 ambient;
		Math::Vector4 diffuse;
		Math::Vector4 specular;
	};

	struct Material // TODO: Add to Graphics library (i.e. Graphics::Material)
	{
		Material()
			: ambient(Math::Vector4::White())
			, diffuse(Math::Vector4::White())
			, specular(Math::Vector4::White())
			, specularPower(0.0f)
		{}

		Math::Vector4 ambient;
		Math::Vector4 diffuse;
		Math::Vector4 specular;
		float specularPower;
	};

	struct ConstantData
	{
		Math::Matrix4 wvp;
	};
	Core::Window mWindow;
	Core::Timer mTimer;

	Graphics::Camera mCamera;
	Graphics::Transform mCameraTransform;

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::MeshBuffer mMeshBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

	Graphics::Texture mTexture;
	Graphics::Sampler mSampler;

	Graphics::Mesh mMesh;
	Graphics::MeshBuilder mMeshBuilder;

	Math::Vector3 mRotation;
};
