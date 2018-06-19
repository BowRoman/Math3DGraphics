#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameObject.h"

const void CameraComponent::CreateFunc(GameObject* gameObj, const TiXmlNode* node)
{
	auto newComponent = gameObj->AddComponent<CameraComponent>();
}

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}
