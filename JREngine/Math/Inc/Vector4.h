#pragma once
#ifndef INCLUDED_MATH_VECTOR4_H
#define INCLUDED_MATH_VECTOR4_H

namespace Math
{

struct Vector4
{
	float x, y, z, w;

	Vector4() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float X, float Y, float Z, float W) :x(X), y(Y), z(Z), w(W) {}
	Vector4(const Vector4& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}

	operator float*()
	{
		return (float*)this;
	}

	void Set(float a, float b, float c, float d = 1.0f) { x = a; y = b; z = c; w = d; }

	float LengthSq() const { return x*x + y*y + z*z; }
	float Length() const { return sqrt(LengthSq()); }

	// static functions to return basic colours

	static Vector4 Black()	{ return Vector4(0.0f, 0.0f, 0.0f, 1.0f); }
	static Vector4 White()	{ return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
	static Vector4 Red()	{ return Vector4(1.0f, 0.0f, 0.0f, 1.0f); }
	static Vector4 Green()	{ return Vector4(0.0f, 1.0f, 0.0f, 1.0f); }
	static Vector4 Blue()	{ return Vector4(0.0f, 0.0f, 1.0f, 1.0f); }

	// Operators

	Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vector4& operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	Vector4& operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	Vector4& operator+=(const float s) { x += s; y += s; z += s; w += s; return *this; }
	Vector4& operator-=(const float s) { x -= s; y -= s; z -= s; w -= s; return *this; }
	Vector4& operator*=(const float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4& operator/=(const float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

	friend bool operator==(const Vector4& L, const Vector4& R)
	{
		return (L.x == R.x && L.y == R.y && L.z == R.z && L.w == R.w);
	}
	friend bool operator!=(const Vector4& L, const Vector4& R) { return !(L == R); }

	/*Check which Vectors are closer or further from the origin.*/
	// friend bool operator>(const Vector4& L, const Vector4& R) { return LengthSq(L) < LengthSq(R); }
	// friend bool operator>=(const Vector4& L, const Vector4& R) { return !(L > R); }
	// friend bool operator<(const Vector4& L, const Vector4& R) { return R < L; }
	// friend bool operator<=(const Vector4& L, const Vector4& R) { return !(R < L); }

	Vector4 operator-() const { return Vector4(-x, -y, -z, w); }

	Vector4 operator*(float s) { Vector4 tmp(*this); tmp.x *= s; tmp.y *= s; tmp.z *= s; tmp.w *= s;  return tmp; }
	Vector4 operator/(float s) { Vector4 tmp(*this); tmp.x /= s; tmp.y /= s; tmp.z /= s; tmp.w /= s;  return tmp; }
};

// Vector4 operator*(float s, const Vector4& v) { return Vector4(v) *= s; }
// Vector4 operator*(const Vector4& v, float s) { return Vector4(v) *= s; }
// 
// Vector4 operator-(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
// Vector4 operator+(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }

// Product functions
// float DotProduct(const Vector4& a, const Vector4& b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); }
// Vector4 CrossProduct(const Vector4& a, const Vector4& b)
// {
// 	return Vector4(a.y * b.z - a.z * b.y,
// 				   a.z * b.x - a.x * b.z,
// 				   a.x * b.y - a.y * b.x);
// }

// Return the unit vector of the input
// template<class T> Vector4<T> Normal(const Vector3<T>& a) { double mag = a.Length(); return v<T>(a.x / mag, a.y / mag, a.z / mag); }


}

#endif // #ifndef INCLUDED_MATH_VECTOR4_H