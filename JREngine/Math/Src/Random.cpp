#include "Precompiled.h"

#include "Random.h"

using namespace Math;

void Random::SetSeed(uint32_t seed)
{
	srand(seed);
}

float Random::GetF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

float Random::GetF(float min, float max)
{
	return min + (GetF() * (max - min));
}