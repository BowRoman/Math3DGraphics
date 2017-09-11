#pragma once

#include "Math\Inc\EngineMath.h"

namespace Graphics
{
	class Material
	{
	public:
		Math::Vector4 ambient;
		Math::Vector4 diffuse;
		Math::Vector4 specular;
		float specularPower;

		Material()
			:ambient(Math::Vector4::White())
			, diffuse(Math::Vector4::White())
			, specular(Math::Vector4::White())
			, specularPower(1.0f)
		{}

	};
}