#include "Precompiled.h"
#include "FPControllerComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

#include <Input\Inc\Input.h>

namespace GameEngine
{

void FPControllerComponent::CreateFunc(GameObject* gameObj, const TiXmlNode* node)
{
	auto newComponent = gameObj->AddComponent<FPControllerComponent>();
	auto elem = node->FirstChildElement();
}

FPControllerComponent::FPControllerComponent()
	: mBaseSpeed{ 5.0f }
	, mSpeedMultiplier{ 2.5f }
	, mTurnSpeed{ 2.0f }
{
}

FPControllerComponent::~FPControllerComponent()
{
}

void FPControllerComponent::Update(float dTime)
{
	Input::InputSystem* iS = Input::InputSystem::Get();

	Graphics::Transform& cameraTrans = mGameObject->GetComponent<CameraComponent>()->GetCamera().mTransform;
	
	float speed = mBaseSpeed;
	if (iS->IsKeyDown(Keys::LSHIFT))
	{
		speed *= mSpeedMultiplier;
	}
	if (iS->IsKeyDown(Keys::W))
	{
		cameraTrans.Walk(speed * dTime);
	}
	if (iS->IsKeyDown(Keys::S))
	{
		cameraTrans.Walk(-speed * dTime);
	}
	if (iS->IsKeyDown(Keys::A))
	{
		cameraTrans.Strafe(-speed * dTime);
	}
	if (iS->IsKeyDown(Keys::D))
	{
		cameraTrans.Strafe(speed * dTime);
	}
	if (iS->IsKeyDown(Keys::E))
	{
		cameraTrans.Rise(speed * dTime);
	}
	if (iS->IsKeyDown(Keys::Q))
	{
		cameraTrans.Rise(-speed * dTime);
	}
	if (iS->IsMouseDown(Mouse::RBUTTON))
	{
		cameraTrans.Yaw(iS->GetMouseMoveX() * mTurnSpeed * dTime);
		cameraTrans.Pitch(iS->GetMouseMoveY() * mTurnSpeed * dTime);
	}
}

} // namespace GameEngine