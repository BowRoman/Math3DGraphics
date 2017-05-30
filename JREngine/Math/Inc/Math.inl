template <typename T>
inline T Min(T a, T b)
{
	return (a > b) ? b : a;
}

template <typename T>
inline T Max(T a, T b)
{
	return (a < b) ? b : a;
}

template <typename T>
inline T Clamp(T value, T min, T max)
{
	return Max(min, Min(max, value));
}

inline float Abs(float value)
{
	return (value >= 0.0f) ? value : -value;
}

inline float Sign(float value)
{
	return (value >= 0.0f) ? 1.0f : -1.0f;
}

inline float Sqr(float value)
{
	return value * value;
}

inline float Sqrt(float value)
{
	ASSERT(value >= 0.0f, "[Math] Cannot square root a negative number!");
	return sqrt(value);
}

inline bool Compare(float a, float b, float epsilon)
{
	return Abs(a - b) < epsilon;
}

inline bool IsZero(float value)
{
	return Compare(value, 0.0f);
}

inline bool IsZero(const Vector3& v)
{
	return IsZero(v.x) && IsZero(v.y) && IsZero(v.z);
}

inline bool IsZero(const Vector4& v)
{
	return IsZero(v.x) && IsZero(v.y) && IsZero(v.z) && IsZero(v.w);
}

inline float MagnitudeSqr(const Vector3& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

inline float Magnitude(const Vector3& v)
{
	return Sqrt(MagnitudeSqr(v));
}

inline float Math::MagnitudeXZSqr(const Vector3& v)
{
	return (v.x * v.x) + (v.z * v.z);
}

inline float MagnitudeXZ(const Vector3& v)
{
	return Sqrt(MagnitudeXZSqr(v));
}

inline float MagnitudeSqr(const Vector4& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
}

inline float Magnitude(const Vector4& v)
{
	return Sqrt(MagnitudeSqr(v));
}

inline Vector3 Normalize(const Vector3& v)
{
	ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	const float inv = 1.0f / Magnitude(v);
	return v * inv;
}

inline Vector4 Normalize(const Vector4& v)
{
	ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	const float inv = 1.0f / Magnitude(v);
	return Vector4(v.x * inv, v.y * inv, v.z * inv, v.w * inv);
}

inline Quaternion Normalize(const Quaternion& q)
{
	const float magSqr = (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
	const float magInv = 1.0f / sqrt(magSqr);
	return q * magInv;
}

inline float DistanceSqr(const Vector3& a, const Vector3& b)
{
	return MagnitudeSqr(a - b);
}

inline float Math::Distance(const Vector3& a, const Vector3& b)
{
	return Sqrt(DistanceSqr(a, b));
}

inline float Math::DistanceXZSqr(const Vector3& a, const Vector3& b)
{
	return MagnitudeXZSqr(a - b);
}

inline float DistanceXZ(const Vector3& a, const Vector3& b)
{
	return Sqrt(DistanceXZSqr(a, b));
}

inline float Math::Dot(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return Vector3
	(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x)
	);
}

inline Vector3 Project(const Vector3& v, const Vector3& n)
{
	return n * (Dot(v, n) / Dot(n, n));
}

inline float Math::Determinant(const Matrix4& m)
{
	float det = 0.0f;
	det  = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
	det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
	det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
	det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
	return det;
}

inline Matrix4 Math::Adjoint(const Matrix4& m)
{
	return Matrix4
	(
		 (m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		 (m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

		-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		 (m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		 (m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

		 (m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		 (m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

		-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		 (m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		 (m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
	);
}

inline Matrix4 Inverse(const Matrix4& m)
{
	const float determinant = Determinant(m);
	ASSERT(!IsZero(determinant), "[Math] Cannot find the inverse of matrix. Determinant equals 0.0!");
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

inline Matrix4 Transpose(const Matrix4& m)
{
	return Matrix4
	(
		m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44
	);
}

inline Vector3 GetTranslation(const Matrix4& m)
{
	return Vector3(m._41, m._42, m._43);
}

inline Vector3 GetRight(const Matrix4& m)
{
	return Vector3(m._11, m._12, m._13);
}

inline Vector3 GetUp(const Matrix4& m)
{
	return Vector3(m._21, m._22, m._23);
}

inline Vector3 GetForward(const Matrix4& m)
{
	return Vector3(m._31, m._32, m._33);
}

inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
		v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
		v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
	);
}

inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31,
		v.x * m._12 + v.y * m._22 + v.z * m._32,
		v.x * m._13 + v.y * m._23 + v.z * m._33
	);
}

inline float Lerp(float a, float b, float t)
{
	return a + ((b - a) * t);
}

inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)
{
	return v0 + ((v1 - v0) * t);
}

inline Quaternion Lerp(Quaternion q0, Quaternion q1, float t)
{
	return (q0 * (1.0f - t)) + (q1 * t);
}

inline Vector3 GetPoint(const Ray& ray, float d)
{
	return ray.origin + (ray.direction * d);
}

inline Vector3 GetClosestPoint(const Ray& ray, const Vector3& point)
{
	Vector3 orgToPoint = point - ray.origin;
	float d = Dot(orgToPoint, ray.direction);
	return GetPoint(ray, d);
}