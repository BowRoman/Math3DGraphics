#ifndef INCLUDED_MATH_VECTOR3_H
#define INCLUDED_MATH_VECTOR3_H

namespace Math {

struct Vector3
{
	float x, y, z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float f) : x(f), y(f), z(f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	static Vector3 Zero();
	static Vector3 One();
	static Vector3 Random();
	static Vector3 XAxis();
	static Vector3 YAxis();
	static Vector3 ZAxis();
	
	Vector3 operator-() const;
	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator-(const Vector3& rhs) const;
	Vector3 operator*(const Vector3& rhs) const;
	Vector3 operator*(float s) const;
	Vector3 operator/(float s) const;

	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator-=(const Vector3& rhs);
	Vector3& operator*=(const Vector3& rhs);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	bool operator==(const Vector3& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }
	bool operator!=(const Vector3& rhs) const { return (x != rhs.x && y != rhs.y && z != rhs.z); }
};

#include "Vector3.inl"

} // namespace Math

#endif // #ifndef INCLUDED_MATH_VECTOR3_H