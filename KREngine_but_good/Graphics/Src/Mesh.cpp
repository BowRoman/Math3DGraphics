#include "Precompiled.h"
#include "Mesh.h"
#include "VertexTypes.h"

Graphics::Mesh::Mesh()
	:mVertices(nullptr)
	, mIndices(nullptr)
	, mVertexCount(0)
	, mIndexCount(0)
{
}

Graphics::Mesh::~Mesh()
{
}

Graphics::Vertex* Graphics::Mesh::GetVertices()
{
	return mVertices;
}

uint32_t * Graphics::Mesh::GetIndices()
{
	return mIndices;
}

Graphics::Vertex& Graphics::Mesh::GetVertex(uint32_t index)
{
	return mVertices[index];
}

uint32_t Graphics::Mesh::GetVertexCount()
{
	return mVertexCount;
}

uint32_t Graphics::Mesh::GetIndexCount()
{
	return mIndexCount;
}

void Graphics::Mesh::Allocate(uint32_t vertcount, uint32_t intcount)
{
	ASSERT(mIndices == nullptr, "[Mesh] Mesh already contains indices");
	ASSERT(mVertices == nullptr, "[Mesh] Mesh already contains vertices");
	ASSERT(vertcount != 0, "[Mesh] No vertex allocation size given");
	ASSERT(intcount != 0, "[Mesh] No index allocation size given");
	mVertices = new Vertex[vertcount];
	mVertexCount = vertcount;
	mIndices = new uint32_t[intcount];
	mIndexCount = intcount;
}

void Graphics::Mesh::Terminate()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
}


