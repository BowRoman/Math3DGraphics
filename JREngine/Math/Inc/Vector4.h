#ifndef INCLUDED_MATH_VECTOR4_H
#define INCLUDED_MATH_VECTOR4_H

namespace Math
{

struct Vector4
{
	float x, y, z, w;

	Vector4() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float X, float Y, float Z, float W) :x(X), y(Y), z(Z), w(W) {}

	operator float*()
	{
		return (float*)this;
	}


	// static functions to return basic colours

	static Vector4 Black()	{ return Vector4(0.0f, 0.0f, 0.0f, 1.0f); }
	static Vector4 White()	{ return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
	static Vector4 Red()	{ return Vector4(1.0f, 0.0f, 0.0f, 1.0f); }
	static Vector4 Green()	{ return Vector4(0.0f, 1.0f, 0.0f, 1.0f); }
	static Vector4 Blue()	{ return Vector4(0.0f, 0.0f, 1.0f, 1.0f); }
};

}

#endif // #ifndef INCLUDED_MATH_VECTOR4_H