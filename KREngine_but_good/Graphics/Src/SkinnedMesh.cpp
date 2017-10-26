#include "Precompiled.h"
#include "SkinnedMesh.h"
#include "VertexTypes.h"

Graphics::SkinnedMesh::SkinnedMesh()
	: mVertices(nullptr)
	, mIndices(nullptr)
	, mVertexCount(0)
	, mIndexCount(0)
{
}

Graphics::SkinnedMesh::~SkinnedMesh()
{
}

Graphics::VertexBone* Graphics::SkinnedMesh::GetVertices()
{
	return mVertices;
}

uint32_t * Graphics::SkinnedMesh::GetIndices()
{
	return mIndices;
}

Graphics::VertexBone& Graphics::SkinnedMesh::GetVertex(uint32_t index)
{
	return mVertices[index];
}

uint32_t Graphics::SkinnedMesh::GetVertexCount()
{
	return mVertexCount;
}

uint32_t Graphics::SkinnedMesh::GetIndexCount()
{
	return mIndexCount;
}

void Graphics::SkinnedMesh::Allocate(uint32_t vertcount, uint32_t intcount)
{
	ASSERT(mIndices == nullptr, "[SkinnedMesh] SkinnedMesh already contains indices");
	ASSERT(mVertices == nullptr, "[SkinnedMesh] SkinnedMesh already contains vertices");
	ASSERT(vertcount != 0, "[SkinnedMesh] No vertex allocation size given");
	ASSERT(intcount != 0, "[SkinnedMesh] No index allocation size given");
	mVertices = new VertexBone[vertcount];
	mVertexCount = vertcount;
	mIndices = new uint32_t[intcount];
	mIndexCount = intcount;
}

void Graphics::SkinnedMesh::Terminate()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
}


