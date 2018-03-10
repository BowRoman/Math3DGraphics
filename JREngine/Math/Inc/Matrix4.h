#ifndef INCLUDED_MATH_MATRIX_H
#define INCLUDED_MATH_MATRIX_H

namespace Math {

struct Matrix4
{
	union
	{
		float data[16];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
	};

	Matrix4()
		: _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
		, _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
		, _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
		, _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
	{}

	Matrix4(float _11, float _12, float _13, float _14,
		   float _21, float _22, float _23, float _24,
		   float _31, float _32, float _33, float _34,
		   float _41, float _42, float _43, float _44)
		: _11(_11), _12(_12), _13(_13), _14(_14)
		, _21(_21), _22(_22), _23(_23), _24(_24)
		, _31(_31), _32(_32), _33(_33), _34(_34)
		, _41(_41), _42(_42), _43(_43), _44(_44)
	{}
	
	static Matrix4 Zero();
	static Matrix4 Identity();
	static Matrix4 Translation(float x, float y, float z);
	static Matrix4 Translation(const Vector3& v);
	static Matrix4 RotationX(float rad);
	static Matrix4 RotationY(float rad);
	static Matrix4 RotationZ(float rad);
	static Matrix4 RotationAxis(const Vector3& axis, float rad);
	static Matrix4 RotationQuaternion(const Quaternion& q);
	static Matrix4 Scaling(float s);
	static Matrix4 Scaling(float sx, float sy, float sz);
	static Matrix4 Scaling(const Vector3& s);
	
	Matrix4 operator-() const;

	Matrix4 operator+(const Matrix4& rhs) const;
	Matrix4 operator-(const Matrix4& rhs) const;
	Matrix4 operator*(const Matrix4& rhs) const;
	Matrix4 operator*(float s) const;
	Matrix4 operator/(float s) const;

	Matrix4 operator+=(const Matrix4& rhs);
};

#include "Matrix4.inl"

} // namespace Math

#endif // #ifndef INCLUDED_MATH_MATRIX_H