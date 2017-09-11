#pragma once

#include "VertexTypes.h"

namespace Graphics
{
	class Mesh
	{

	public:
		Vertex* mVertices;
		uint32_t mVertexCount;

		uint32_t* mIndices;
		uint32_t mIndexCount;

	public:
		Mesh();
		~Mesh();

		Vertex* GetVertices();
		uint32_t* GetIndices();

		Vertex& GetVertex(uint32_t index);

		uint32_t GetVertexCount();
		uint32_t GetIndexCount();
		
		void Allocate(uint32_t vertcount, uint32_t intcount);
		void Terminate();
	};
}
