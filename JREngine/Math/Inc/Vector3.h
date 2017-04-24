#pragma once
#ifndef INCLUDED_MATH_VECTOR3_H
#define INCLUDED_MATH_VECTOR3_H

namespace Math
{

template<typename T>
class Vector3 {
public:
	T x, y, z;

	Vector3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vector3(T a, T b, T c) : x(a), y(b), z(c) {}

	void Set(T a, T b, T c) { x = a; y = b; z = c; }

	// double Length() const { return sqrt(x*x + y*y, z*z); }
	double LengthSq()const { return x*x + y*y + z*z; }

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
	friend bool operator>(const Vector3& L, const Vector3& R) { return LengthSq(L) < LengthSq(R); }
	friend bool operator>=(const Vector3& L, const Vector3& R) { return !(L > R); }
	friend bool operator<(const Vector3& L, const Vector3& R) { return R < L; }
	friend bool operator<=(const Vector3& L, const Vector3& R) { return !(R < L); }

	Vector3 operator-() const { return Vector3(-x, -y, -z); }

	Vector3 operator*(T s) { Vector3 tmp(*this); tmp.x *= s; tmp.y *= s; tmp.z *= s;  return tmp; }
	Vector3 operator/(T s) { Vector3 tmp(*this); tmp.x /= s; tmp.y /= s; tmp.z /= s;  return tmp; }
};

template<class T> Vector3<T> operator*(const T& s, const Vector3<T>& v) { return Vector3<T>(v) *= s; }
template<class T> Vector3<T> operator*(const Vector3<T>& v, const T& s) { return Vector3<T>(v) *= s; }

template<class T> Vector3<T>  operator-(const Vector3<T>& v1, const Vector3<T>& v2) { return Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
template<class T> Vector3<T>  operator+(const Vector3<T>& v1, const Vector3<T>& v2) { return Vector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

// Product functions
template<class T> T DotProduct(const Vector3<T>& a, const Vector3<T>& b) { return  ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); }
template<class T>  Vector3<T>  CrossProduct(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(a.y * b.z - a.z * b.y,
					  a.z * b.x - a.x * b.z,
					  a.x * b.y - a.y * b.x);
}

// Return the unit vector of the input
// template<class T> Vector3<T> Normal(const Vector3<T>& a) { double mag = a.Length(); return v<T>(a.x / mag, a.y / mag, a.z / mag); }

}

#endif // #ifndef INCLUDED_MATH_VECTOR3_H