#pragma once

#include "Mesh.h"
#include "VertexTypes.h"

namespace Graphics
{
	class MeshBuilder
	{
	public:
		static void GeneratePlane(Mesh& mesh, const float width, const float length, const float height);
		static void GenerateStackSliceSphere(Mesh &mesh, const uint32_t maxStacks, const uint32_t maxSlices, const float radius);
	};
}