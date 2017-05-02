#include "Precompiled.h"
#include "EngineMath.h"

namespace Math
{
	Vector3 operator*(const Vector3& v, const Matrix4& m)
	{
		return Vector3(m[0] * v.x + m[1] * v.y + m[2] * v.z,
					   m[4] * v.x + m[5] * v.y + m[6] * v.z,
					   m[8] * v.x + m[9] * v.y + m[10] * v.z);
	}

	Vector3 operator*(float s, const Vector3& v) { return Vector3(v) *= s; }
	Vector3 operator*(const Vector3& v, float s) { return Vector3(v) *= s; }

	Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
}