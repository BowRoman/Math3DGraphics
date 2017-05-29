#ifndef INCLUDED_MATH_OBB_H
#define INCLUDED_MATH_OBB_H

namespace Math {

struct OBB
{
	Vector3 center;
	Vector3 extend;
	Quaternion rot;

	OBB()
		: center(0.0f, 0.0f, 0.0f)
		, extend(1.0f, 1.0f, 1.0f)
		, rot(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	OBB(float x, float y, float z, float sx, float sy, float sz)
		: center(x, y, z)
		, extend(sx, sy, sz)
		, rot(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	OBB(float x, float y, float z, float sx, float sy, float sz, float ax, float ay, float az, float angle)
		: center(x, y, z)
		, extend(sx, sy, sz)
		, rot(Quaternion::RotationAxis(Math::Vector3(ax, ay, az), angle))
	{}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_OBB_H