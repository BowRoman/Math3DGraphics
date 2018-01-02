#include "Precompiled.h"
#include "SkinnedMesh.h"
#include "VertexTypes.h"

Graphics::SkinnedMesh::SkinnedMesh()
	: mVertices(nullptr)
	, mIndices(nullptr)
	, mNumVertices(0)
	, mNumIndices(0)
{
}

Graphics::SkinnedMesh::~SkinnedMesh()
{
}

void Graphics::SkinnedMesh::Allocate(uint32_t vertcount, uint32_t intcount)
{
	ASSERT(mIndices == nullptr, "[SkinnedMesh] SkinnedMesh already contains indices");
	ASSERT(mVertices == nullptr, "[SkinnedMesh] SkinnedMesh already contains vertices");
	ASSERT(vertcount != 0, "[SkinnedMesh] No vertex allocation size given");
	ASSERT(intcount != 0, "[SkinnedMesh] No index allocation size given");
	mVertices = new VertexBone[vertcount];
	mNumVertices = vertcount;
	mIndices = new uint32_t[intcount];
	mNumIndices = intcount;
}

void Graphics::SkinnedMesh::Terminate()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
}


