#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameObject.h"

namespace GameEngine
{

void CameraComponent::CreateFunc(GameObject* gameObj, const TiXmlNode* node)
{
	auto newComponent = gameObj->AddComponent<CameraComponent>();
	newComponent->mCamera.mTransform.SetDirection({ 0.0f, 0.0f, 1.0f });
}

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float dTime)
{

}

} // namespace GameEngine