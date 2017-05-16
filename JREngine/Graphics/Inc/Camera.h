#ifndef INCLUDED_GRAPHICS_CAMERA_H
#define INCLUDED_GRAPHICS_CAMERA_H

namespace Graphics
{

class Transform;

class Camera
{
public:
	Camera();
	Camera(float nearPlane, float farPlane, float FOV);
	~Camera() = default;

	void Setup(float nearPlane, float farPlane, float FOV);

	void SetFOVLimits(float min, float max);
	void ZoomIn(float rad);
	void ZoomOut(float rad);

	Math::Matrix4 GetViewMatrix(const Transform& transform);
	Math::Matrix4 GetProjectionMatrix(float aspectRatio);

private:
	float mNearPlane;
	float mFarPlane;
	float mFOV;
	float mMinFOV;
	float mMaxFOV;
}; // class Camera

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_CAMERA_H