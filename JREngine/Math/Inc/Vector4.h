#ifndef INCLUDED_MATH_VECTOR4_H
#define INCLUDED_MATH_VECTOR4_H

#include "Vector3.h"

namespace Math {

struct Vector4
{
	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(const Vector3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
	operator float*() { return (float*)this; }

	static Vector4 Black();
	static Vector4 Gray();
	static Vector4 White();
	static Vector4 Red();
	static Vector4 Green();
	static Vector4 Blue();
	static Vector4 Yellow();
	static Vector4 Magenta();
	static Vector4 Cyan();
	static Vector4 Orange();
	static Vector4 Lime();
	static Vector4 CyanGreen();
	static Vector4 Azure();
	static Vector4 Violet();
	static Vector4 Strobe();
	static Vector4 Rose();

	Vector4 operator*(float s) const;
};

#include "Vector4.inl"

} // namespace Math

#endif // #ifndef INCLUDED_MATH_VECTOR4_H