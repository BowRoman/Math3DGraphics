#include "Precompiled.h"
#include "Transform.h"
#include "Camera.h"



Graphics::Camera::Camera()
	:mNear(0.1f),
	mFar(100000.0f),
	mFieldOfView(60.0f * Math::kDegToRad),
	mMinFOV(0.0f),
	mMaxFOV(180.0f * Math::kDegToRad)
{
}

Graphics::Camera::Camera(float nearPlane, float farPlane, float fov)
{
	Setup(nearPlane, farPlane, fov);
}

void Graphics::Camera::Setup(float nearPlane, float farPlane, float fov)
{
	ASSERT(nearPlane >= 0.0f, "[Camera] Invalid nearPlane");
	ASSERT(farPlane > 0.0f, "[Camera] Invalid farPlane");
	ASSERT(farPlane > nearPlane, "[Camera] nearPlane greater than farPlane");
	ASSERT(fov > 0.0f, "[Camera] Invalid FOV");

	mNear = nearPlane;
	mFar = farPlane;
	mFieldOfView = fov;
}

void Graphics::Camera::SetFOVLimits(float _min, float _max)
{
	ASSERT(_min < _max, "[Camera] Invalid FOV Limits");
	ASSERT(_max < 180.0f * Math::kDegToRad, "[Camera] Invalid FOV Limits");
	ASSERT(_min > 0.0f, "[Camera] Invalid FOV Limits");

	mMinFOV = _min;
	mMaxFOV = _max;
}

void Graphics::Camera::ZoomIn(float rad)
{
	ASSERT(rad > 0.0f, "[Camera] Invalid zoom in.");
	float tempFloat = mFieldOfView - rad;
	mFieldOfView = max(tempFloat, mMinFOV);
}

void Graphics::Camera::ZoomOut(float rad)
{
	ASSERT(rad < 180.0f * Math::kDegToRad, "[Camera] Invalid zoom out.");
	float tempFloat = mFieldOfView + rad;
	mFieldOfView = min(tempFloat, mMaxFOV);
}

Math::Matrix4 Graphics::Camera::GetViewMatrix()
{
	return Math::Inverse(mTransform.GetWorldMatrix());
}

Math::Matrix4 Graphics::Camera::GetViewMatrix(const Transform& transform)
{
	return Math::Inverse(transform.GetWorldMatrix());
}


Math::Matrix4 Graphics::Camera::GetProjectionMatrix(float aspect)
{
	const float h = 1 / tan(mFieldOfView * 0.5f);
	const float w = h / aspect;
	const float n = mNear;
	const float f = mFar;
	const float d = f / (f - n);

	return Math::Matrix4
	(
		w, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, d, 1.0f,
		0.0f, 0.0f, -n * d, 0.0f
	);
}


Math::Ray Graphics::Camera::ScreenPointToRay(int screenX, int screenY, uint32_t screenWidth, uint32_t screenHeight) const
{
	const float aspect = (float)screenWidth / (float)screenHeight;
	const float halfWidth = screenWidth * 0.5f;
	const float halfHeight = screenHeight * 0.5f;
	const float tanFOV = tanf(mFieldOfView * 0.5f);
	const float dx = tanFOV * ((float)screenX / halfWidth - 1.0f) * aspect;
	const float dy = tanFOV * (1.0f - (float)screenY / halfHeight);

	Math::Ray ray;
	ray.origin = Math::Vector3::Zero();
	ray.direction = Normalize(Math::Vector3(dx, dy, 1.0f));


	Math::Matrix4 invMatView = mTransform.GetWorldMatrix();
	ray.origin = TransformCoord(ray.origin, invMatView);
	ray.direction = TransformNormal(ray.direction, invMatView);
	return ray;
}