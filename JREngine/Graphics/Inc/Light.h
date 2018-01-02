#pragma once

#include "Math\Inc\EngineMath.h"

namespace Graphics
{

class Light
{
public:
	Math::Vector3 direction;
	float padding;
	Math::Vector4 ambient;
	Math::Vector4 diffuse;
	Math::Vector4 specular;

	Light()
		: direction(Math::Vector3::ZAxis())
		, ambient(Math::Vector4::White())
		, diffuse(Math::Vector4::White())
		, specular(Math::Vector4::White())
	{}

}; // class Light

} // namespace Graphics