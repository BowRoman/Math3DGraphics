#pragma once

#include "Component.h"

namespace GameEngine
{

class FPControllerComponent : public Component
{
	float mBaseSpeed;
	float mSpeedMultiplier;
	float mTurnSpeed;

public:
	REGISTER_TYPE(FPCC); // (FPC)ontroller(C)omponent

	static void CreateFunc(GameObject* gameObj, const TiXmlNode* node);

	FPControllerComponent();
	~FPControllerComponent() override;

	void Update(float dTime) override;

};

} // namespace GameEngine