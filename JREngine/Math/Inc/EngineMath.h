#pragma once
#ifndef INCLUDED_MATH_ENGINEMATH_H
#define INCLUDED_MATH_ENGINEMATH_H

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

// TODO: add helper functions/operators (e.g. vector * matrix)
// TODO: add Vector3.h, Vector4, Matrix4.h, and their operators and functions

namespace Math
{
	// TODO: add interconected operators and functinos

	void Matrix4::SetRow(int index, const Vector4& v);
	void Matrix4::SetRow(int index, const Vector3& v);
}

#endif // #ifndef INCLUDED_MATH_ENGINEMATH_H