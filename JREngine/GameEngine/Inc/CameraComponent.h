#pragma once

#include "Component.h"
#include "Graphics\Inc\Camera.h"

namespace GameEngine
{

class CameraComponent : public Component
{
	struct ConstantData
	{
		Math::Matrix4 wvp;
	};

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::PixelShader mPixelShader;
	Graphics::VertexShader mVertexShader;
	Graphics::MeshBuffer mMeshBuffer;

	Graphics::Camera mCamera;

public:
	REGISTER_TYPE(CAMC); // (Cam)era(C)omponent

	static void CreateFunc(GameObject* gameObj, const TiXmlNode* node);

	CameraComponent();
	~CameraComponent() override;

	void Update(float dTime) override;

	const Graphics::Camera& GetCamera() const { return mCamera; }
	Graphics::Camera& GetCamera() { return mCamera; }

};

} // namespace GameEngine