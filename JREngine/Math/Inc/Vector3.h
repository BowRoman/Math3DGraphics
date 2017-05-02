#pragma once
#ifndef INCLUDED_MATH_VECTOR3_H
#define INCLUDED_MATH_VECTOR3_H

namespace Math
{

class Vector3 {
public:
	float x, y, z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

	void Set(float a, float b, float c) { x = a; y = b; z = c; }

	float LengthSq() const { return x*x + y*y + z*z; }
	float Length() const { return sqrt(LengthSq()); }

	// Operators

	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector3& operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

	Vector3& operator+=(const float s) { x += s; y += s; z += s; return *this; }
	Vector3& operator-=(const float s) { x -= s; y -= s; z -= s; return *this; }
	Vector3& operator*=(const float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator/=(const float s) { x /= s; y /= s; z /= s; return *this; }

	friend bool operator==(const Vector3& L, const Vector3& R)
	{
		return (L.x == R.x && L.y == R.y && L.z == R.z);
	}
	friend bool operator!=(const Vector3& L, const Vector3& R) { return !(L == R); }

	/*Check which Vectors are closer or further from the origin.*/
	friend bool operator>(const Vector3& L, const Vector3& R) { return L.LengthSq() < R.LengthSq(); }
	friend bool operator>=(const Vector3& L, const Vector3& R) { return !(L > R); }
	friend bool operator<(const Vector3& L, const Vector3& R) { return R < L; }
	friend bool operator<=(const Vector3& L, const Vector3& R) { return !(R < L); }

	Vector3 operator-() const { return Vector3(-x, -y, -z); }

	Vector3 operator*(float s) { Vector3 tmp(*this); tmp.x *= s; tmp.y *= s; tmp.z *= s;  return tmp; }
	Vector3 operator/(float s) { Vector3 tmp(*this); tmp.x /= s; tmp.y /= s; tmp.z /= s;  return tmp; }

	// Product functions
	float Dot(const Vector3& b) { return  ((x * b.x) + (y * b.y) + (z * b.z)); }
	Vector3 Cross(const Vector3& b)
	{
		return Vector3(y * b.z - z * b.y,
					   z * b.x - x * b.z,
					   x * b.y - y * b.x);
	}

	// Return the unit vector of the input
	Vector3 Normal() { float mag = Length(); return Vector3(x / mag, y / mag, z / mag); }
};

}

#endif // #ifndef INCLUDED_MATH_VECTOR3_H