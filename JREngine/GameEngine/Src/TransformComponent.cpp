#include "Precompiled.h"
#include "TransformComponent.h"

#include "GameObject.h"

const void TransformComponent::CreateFunc(GameObject* gameObj, const TiXmlNode* node)
{
	auto newComponent = gameObj->AddComponent<TransformComponent>();
	auto vec = node->FirstChildElement();
	while (vec)
	{
		auto dim = vec->FirstChildElement();
		float x = static_cast<float>(std::atof(dim->GetText()));
		float y = static_cast<float>(std::atof(dim->NextSiblingElement()->GetText()));
		float z = static_cast<float>(std::atof(dim->NextSiblingElement()->GetText()));

		if (std::strcmp(vec->FirstAttribute()->Value(), "Position"))
		{
			newComponent->SetPosition({ x,y,z });
		}
		if (std::strcmp(vec->FirstAttribute()->Value(), "Forward"))
		{
			newComponent->SetForward({ x,y,z });
		}
		vec->NextSiblingElement();
	}
}

TransformComponent::TransformComponent()
	: mPosition(Math::Vector3::Zero())
	, mForward(Math::Vector3::ZAxis())
{
}

TransformComponent::~TransformComponent()
{
}

Math::Matrix4 TransformComponent::GetLocalToWorld() const
{
	Math::Vector3 yAxis = Math::Vector3::YAxis();
	Math::Vector3 forward = Math::Normalize(mForward);
	Math::Vector3 side = Math::Cross(yAxis, forward);
	Math::Vector3 up = Math::Cross(forward, side);
	Math::Vector3 pos = mPosition;

	return Math::Matrix4
	(
		side.x, side.y, side.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		forward.x, forward.y, forward.z, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
}