#include "Precompiled.h"
#include "Transform.h"



Graphics::Transform::Transform()
	:mPosition(0.0,0.0,0.0),
	mDirection(0.0,0.0,1.0)
{

}

Graphics::Transform::Transform(const Math::Vector3 & position, const Math::Vector3 & direction)
	:mPosition(position),
	mDirection(Math::Normalize(direction))
{

}

Graphics::Transform::~Transform()
{

}

void Graphics::Transform::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Graphics::Transform::Strafe(float distance)
{
	mPosition += Math::Cross(Math::Vector3::YAxis(), mDirection) * distance;
}

void Graphics::Transform::Rise(float distance)
{
	mPosition += Math::Vector3::YAxis() * distance;
}

void Graphics::Transform::Yaw(float rad)
{
	Math::Matrix4 rotationY = Math::Matrix4::RotationY(rad);
	mDirection = Math::TransformNormal(mDirection, rotationY);
}

void Graphics::Transform::Pitch(float rad)
{
	Math::Vector3 right = Math::Cross(Math::Vector3::YAxis(), mDirection);
	Math::Matrix4 rotationPitch = Math::Matrix4::RotationAxis(right, rad);
	mDirection = Math::TransformNormal(mDirection, rotationPitch);
}

Math::Matrix4 Graphics::Transform::GetWorldMatrix() const
{
	Math::Vector3 forward = Math::Normalize(mDirection);
	Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis(), forward));
	Math::Vector3 up = Math::Normalize(Math::Cross(forward, right));
	return Math::Matrix4(
		right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		forward.x, forward.y, forward.z, 0.0f,
		mPosition.x, mPosition.y, mPosition.z, 1.0f);
}



void Graphics::Transform::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
}

void Graphics::Transform::SetDirection(const Math::Vector3 & direction)
{
	mDirection = direction;
}

