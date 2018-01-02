#ifndef INCLUDED_MATH_ENGINEMATH_H
#define INCLUDED_MATH_ENGINEMATH_H

#include "Common.h"
#include "Random.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"

// 3D
#include "AABB.h"
#include "OBB.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"

// 2D
#include "Circle.h"
#include "Rect.h"

namespace Math {

extern const float kPi;
extern const float kTwoPi;
extern const float kPiByTwo;
extern const float kRootTwo;
extern const float kRootThree;
extern const float kDegToRad;
extern const float kRadToDeg;
extern const float KEpsilon;

template <typename T> T Min(T a, T b);
template <typename T> T Max(T a, T b);
template <typename T> T Clamp(T value, T min, T max);

float Abs(float value);
float Sign(float value);
float Sqr(float value);
float Sqrt(float value);

bool Compare(float a, float b, float epsilon = FLT_MIN);

bool IsZero(float value);
bool IsZero(const Vector3& v);
bool IsZero(const Vector4& v);

float MagnitudeSqr(const Vector3& v);
float Magnitude(const Vector3& v);
float MagnitudeXZSqr(const Vector3& v);
float MagnitudeXZ(const Vector3& v);

float MagnitudeSqr(const Vector4& v);
float Magnitude(const Vector4& v);

Vector3 Normalize(const Vector3& v);
Vector4 Normalize(const Vector4& v);
Quaternion Normalize(const Quaternion& q);

float DistanceSqr(const Vector3& a, const Vector3& b);
float Distance(const Vector3& a, const Vector3& b);

float DistanceXZSqr(const Vector3& a, const Vector3& b);
float DistanceXZ(const Vector3& a, const Vector3& b);

float Dot(const Vector3& a, const Vector3& b);
Vector3 Cross(const Vector3& a, const Vector3& b);

Vector3 Project(const Vector3& v, const Vector3& n);

float Determinant(const Matrix4& m);
Matrix4 Adjoint (const Matrix4& m);
Matrix4 Inverse(const Matrix4& m);
Matrix4 Transpose(const Matrix4& m);

Vector3 GetTranslation(const Matrix4& m);
Vector3 GetRight(const Matrix4& m);
Vector3 GetUp(const Matrix4& m);
Vector3 GetForward(const Matrix4& m);

Vector3 TransformCoord(const Vector3& v, const Matrix4& m);
Vector3 TransformNormal(const Vector3& v, const Matrix4& m);

float Lerp(float a, float b, float t);
Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t);
Quaternion Lerp(Quaternion q0, Quaternion q1, float t);
Quaternion Slerp(Quaternion q0, Quaternion q1, float t);

Matrix4 GetTransform(const OBB& obb);

bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance);
bool Intersect(const Ray& ray, const Plane& plane, float& distance);
bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);
bool Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit);
bool Intersect(const Vector3& point, const AABB& aabb);
bool Intersect(const Vector3& point, const OBB& obb);

void GetCorners(const OBB& obb, std::vector<Vector3>& corners);
bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal);

Vector3 GetPoint(const Ray& ray, float d);
Vector3 GetClosestPoint(const Ray& ray, const Vector3& point);

Vector3 Mean(const Vector3* v, uint32_t count);

#include "Math.inl"

} // namespace Math

#endif // #ifndef INCLUDED_MATH_ENGINEMATH_H