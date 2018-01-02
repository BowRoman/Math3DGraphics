#pragma once

#include "Math\Inc\EngineMath.h"
#include "Graphics\Inc\Transform.h"


namespace Graphics
{

	class Camera
	{
	public:
		Camera();
		Camera(float nearPlane, float farPlane, float fov);
		~Camera() {};

		void Setup(float nearPlane, float farPlane, float fov);

		void SetFOVLimits(float min, float max);
		void ZoomIn(float rad);
		void ZoomOut(float rad);

		Math::Matrix4 GetViewMatrix(const Transform& transform); 
		Math::Matrix4 GetViewMatrix();
		Math::Matrix4 GetProjectionMatrix(float aspect);

		Math::Ray ScreenPointToRay(int screenX, int screenY, uint32_t screenWidth, uint32_t screenHeight) const;

		Graphics::Transform mTransform;

	private:
		float mNear;
		float mFar;
		float mFieldOfView;
		float mMinFOV;
		float mMaxFOV;
	}; // class Camera

} // namespace Graphics