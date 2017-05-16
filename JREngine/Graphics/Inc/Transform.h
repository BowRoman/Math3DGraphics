#ifndef INCLUDED_GRAPHICS_TRANSFORM_H
#define INCLUDED_GRAPHICS_TRANSFORM_H

namespace Graphics
{

class Transform
{
public:
	Transform();
	Transform(const Math::Vector3& position, const Math::Vector3& direction);
	~Transform() = default;

	// Translation
	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);

	// Rotation
	void Yaw(float rad);
	void Pitch(float rad);

	Math::Matrix4 GetWorldMatrix() const;

	// Mutators
	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);

	// Accessors
	const Math::Vector3 GetPosition() const				{ return mPosition; }
	const Math::Vector3 GetDirection() const			{ return mDirection; }

private:
	Math::Vector3 mPosition;
	Math::Vector3 mDirection;
}; // class Transform

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_TRANSFORM_H