#include "Precompiled.h"
#include "EngineMath.h"

using namespace Math;

const float Math::kPi = 3.14159265358979f;
const float Math::kTwoPi = 6.28318530717958f;
const float Math::kPiByTwo = 1.57079632679489f;
const float Math::kRootTwo = 1.41421356237309f;
const float Math::kRootThree = 1.73205080756887f;
const float Math::kDegToRad = kPi / 180.0f;
const float Math::kRadToDeg = 180.0f / kPi;

Vector3 Math::operator*(const Vector3& v, const Matrix4& m)
{
	return Vector3(m[0] * v.x + m[1] * v.y + m[2] * v.z,
				   m[4] * v.x + m[5] * v.y + m[6] * v.z,
				   m[8] * v.x + m[9] * v.y + m[10] * v.z);
}

Vector3 Math::operator*(float s, const Vector3& v) { return Vector3(v) *= s; }
Vector3 Math::operator*(const Vector3& v, float s) { return Vector3(v) *= s; }

Vector3 Math::operator-(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
Vector3 Math::operator+(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

Vector3 Math::Cross(const Vector3 & vector1, const Vector3 & vector2)
{
	return Vector3(	vector1.y * vector2.z - vector1.z * vector2.y,
					vector1.z * vector2.x - vector1.x * vector2.z,
					vector1.x * vector2.y - vector1.y * vector2.x);
}

Vector3 Math::Normal(const Vector3 & vector1)
{
	return vector1.Normal();
}

Vector3 Math::TransformNormal(const Vector3& v, const Matrix4& m)
{
	return Vector3
	(
		v.x * m[0] + v.y * m[4] + v.z * m[8],
		v.x * m[1] + v.y * m[5] + v.z * m[9],
		v.x * m[2] + v.y * m[6] + v.z * m[10]
	);
}

Matrix4 Math::RotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = axis.Normal();
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return Matrix4
	(
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

Matrix4 Math::Inverse(const Matrix4& mat)
{
	return Matrix4(mat).Invert();
}