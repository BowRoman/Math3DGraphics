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
	Graphics::MeshBuffer mMeshBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;

	std::vector<Math::Matrix4> mBoneWorldTransforms;
	Graphics::TypedConstantBuffer<Math::Matrix4[64]> mBoneConstantBuffer;
	Graphics::VertexShader mSkinningVertexShader;
	Graphics::PixelShader mSkinningPixelShader;

	Math::Vector3 mRotation;

	float bendElbow = 0, bendWrist = 0, bendShoulder = 0;
};
