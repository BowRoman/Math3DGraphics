#include "Precompiled.h"
#include "TransformComponent.h"

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