#ifndef INCLUDED_MATH_CIRCLE_H
#define INCLUDED_MATH_CIRCLE_H

namespace Math {

struct Circle
{
	Math::Vector2 center;
	float radius;

	Circle() : center(0.0f, 0.0f), radius(1.0f) {}
	Circle(const Math::Vector2& center, float radius) : center(center), radius(radius) {}
	Circle(float x, float y, float radius) : center(x, y), radius(radius) {}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_CIRCLE_H