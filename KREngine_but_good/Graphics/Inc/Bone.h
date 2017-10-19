#pragma once
#include "Common.h"

namespace Graphics
{

struct Bone
{
	Bone()
		: parent(nullptr)
		, index(0)
		, parentIndex(-1)
	{}

	std::string name;
	uint32_t index;

	int parentIndex;
	Bone* parent;

	std::vector<uint32_t> childrenIndex;
	std::vector<Bone*> children;

	Math::Matrix4 transform;
	Math::Matrix4 offsetTransform;
}; // struct Bone

} // namespace Graphics