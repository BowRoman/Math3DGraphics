#ifndef INCLUDED_MATH_VECTOR2_H
#define INCLUDED_MATH_VECTOR2_H

namespace Math {

struct Vector2
{
	float x, y;

	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float x, float y) : x(x), y(y) {}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_VECTOR2_H