#ifndef INCLUDED_MATH_RAY_H
#define INCLUDED_MATH_RAY_H

namespace Math {

struct Ray
{
	Vector3 origin;
	Vector3 direction;

	Ray() : origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 1.0f) {}
	Ray(float x, float y, float z, float dx, float dy, float dz) : origin(x, y, z), direction(dx, dy, dz) {}
	Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction) {}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_RAY_H