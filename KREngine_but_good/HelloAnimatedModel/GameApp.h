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

	void DrawBones(Graphics::Bone* bone, std::vector<Math::Matrix4> matVec);

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

	Graphics::AnimatedModel mModel;

	float mRotation;
};
