#ifndef INCLUDED_MATH_RECT_H
#define INCLUDED_MATH_RECT_H

namespace Math {

struct Rect
{
	float left, top, right, bottom;

	Rect() : left(0.0f), top(0.0f), right(1.0f), bottom(1.0f) {}
	Rect(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}
};

} // namespace Math

#endif // #ifndef INCLUDED_MATH_RECT_H