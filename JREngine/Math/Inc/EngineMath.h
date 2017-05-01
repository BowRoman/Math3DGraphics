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
	// TODO: add interconected operators and functions

	Vector3 operator*(const Vector3& v, const Matrix4& m);

	// forward declare and put in .cpp

	// #include "EngineMath.inl"
}

#endif // #ifndef INCLUDED_MATH_ENGINEMATH_H