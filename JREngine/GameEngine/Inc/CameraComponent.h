#pragma once

#include "Component.h"
#include "Graphics\Inc\Camera.h"

class CameraComponent : public Component
{
	Graphics::Camera mCamera;

public:
	REGISTER_TYPE(CAMC); // (Cam)era(C)omponent

	const static void CreateFunc(GameObject* gameObj, const TiXmlNode* node);

	CameraComponent();
	~CameraComponent() override;

	const Graphics::Camera& GetCamera() const { return mCamera; }
	Graphics::Camera& GetCamera() { return mCamera; }

};