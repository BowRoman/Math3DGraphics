#ifndef INCLUDED_MATH_RANDOM_H
#define INCLUDED_MATH_RANDOM_H

namespace Math {
namespace Random {

void SetSeed(uint32_t seed);

float GetF();
float GetF(float min, float max);

} // namespace Random
} // namespace Math

#endif // #ifndef INCLUDED_MATH_RANDOM_H