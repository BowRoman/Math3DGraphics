#ifndef INCLUDED_MATH_QUATERNION_H
#define INCLUDED_MATH_QUATERNION_H

namespace Math {

struct Quaternion
{
	float x, y, z, w;

	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	static Quaternion Zero();
	static Quaternion Identity();
	
	static Quaternion RotationAxis(const Vector3& axis, float rad);

	Quaternion operator+(const Quaternion& rhs) const;
	Quaternion operator*(float s) const;
};

#include "Quaternion.inl"

} // namespace Math

#endif // #ifndef INCLUDED_MATH_QUATERNION_H