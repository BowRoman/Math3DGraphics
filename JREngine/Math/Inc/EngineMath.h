#pragma once
#ifndef INCLUDED_MATH_ENGINEMATH_H
#define INCLUDED_MATH_ENGINEMATH_H

#include "Common.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

// TODO: add helper functions/operators (e.g. vector * matrix)
// TODO: add Vector3.h, Vector4, Matrix4.h, and their operators and functions

namespace Math
{
extern const float kPi;
extern const float kTwoPi;
extern const float kPiByTwo;
extern const float kRootTwo;
extern const float kRootThree;
extern const float kDegToRad;
extern const float kRadToDeg;

Vector3 operator*(const Vector3& v, const Matrix4& m);

Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);

Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 Cross(const Vector3& vector1, const Vector3& vector2);
Vector3 Normal(const Vector3& vector1);
Vector3 TransformNormal(const Vector3& v, const Matrix4& m);

Matrix4 RotationAxis(const Vector3& axis, float rad);
Matrix4 Inverse(const Matrix4& mat);

// forward declare and put in .cpp
}

#endif // #ifndef INCLUDED_MATH_ENGINEMATH_H