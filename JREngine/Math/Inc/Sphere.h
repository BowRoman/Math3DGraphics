#ifndef INCLUDED_MATH_SPHERE_H
#define INCLUDED_MATH_SPHERE_H

namespace Math {

struct Sphere
{
	Vector3 center;
	float radius;

	Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
	Sphere(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
	Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_SPHERE_H