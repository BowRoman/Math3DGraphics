#ifndef INCLUDED_MATH_PLANE_H
#define INCLUDED_MATH_PLANE_H

namespace Math {

struct Plane
{
	Vector3 n;
	float d;

	Plane() : n(0.0f, 1.0f, 0.0f), d(0.0f) {}
	Plane(float a, float b, float c, float d) : n(a, b, c), d(d) {}
	Plane(Vector3 v, float d) : n(v), d(d) {}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_PLANE_H