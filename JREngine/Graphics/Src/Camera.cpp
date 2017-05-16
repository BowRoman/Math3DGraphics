#include "Precompiled.h"
#include "Camera.h"
#include "Transform.h"

Graphics::Camera::Camera()
	: mNearPlane(0.1f)
	, mFarPlane(10000.0f)
	, mFOV(60.0f * Math::kDegToRad)
	, mMinFOV(0.0f)
	, mMaxFOV(180.0f * Math::kDegToRad)
{
}

Graphics::Camera::Camera(float nearPlane, float farPlane, float FOV)
{
	Setup(nearPlane, farPlane, FOV);
}

void Graphics::Camera::Setup(float nearPlane, float farPlane, float FOV)
{
	ASSERT(nearPlane >= 0.0f, "[Camera] Invalid camera parameters.");
	ASSERT(farPlane > 0.0f, "[Camera] Invalid camera parameters.");
	ASSERT(nearPlane > farPlane, "[Camera] Invalid camera parameters.");
	ASSERT(FOV > 0.0f, "[Camera] Invalid camera parameters.");
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
	mFOV = FOV;
}

void Graphics::Camera::SetFOVLimits(float min, float max)
{
	ASSERT(min > 0.0f, "[Camera] Invalid camera parameters.");
	ASSERT(max < 180.0f * Math::kDegToRad, "[Camera] Invalid camera parameters.");
	ASSERT(min < max, "[Camera] Invalid camera parameters.");
	mMinFOV = min;
	mMaxFOV = max;
}

void Graphics::Camera::ZoomIn(float rad)
{
	ASSERT(rad > 0.0f, "[Camera] Invalid camera parameters.");
	mFOV = std::max(mFOV - rad, mMinFOV);
}

void Graphics::Camera::ZoomOut(float rad)
{
	ASSERT(rad < 180.0f * Math::kDegToRad, "[Camera] Invalid camera parameters.");
	mFOV = std::min(mFOV + rad, mMaxFOV);
}

Math::Matrix4 Graphics::Camera::GetViewMatrix(const Transform& transform)
{
	return Math::Inverse(transform.GetWorldMatrix());
}

Math::Matrix4 Graphics::Camera::GetProjectionMatrix(float aspectRatio)
{
	const float h = 1 / tan(mFOV * 0.5f);
	const float w = h / aspectRatio;
	const float f = mFarPlane;
	const float n = mNearPlane;
	const float d = f / (f - n);
	return Math::Matrix4(
		w,		0.0f,	0.0f,	0.0f,
		0.0f,	h,		0.0f,	0.0f,
		0.0f,	0.0f,	d,		1.0f,
		0.0f,	0.0f,	-n*d,	0.0f
	);
}
