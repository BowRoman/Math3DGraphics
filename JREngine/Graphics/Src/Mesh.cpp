#include "Precompiled.h"
#include "Mesh.h"


using namespace Graphics;

Mesh::Mesh()
	: mVertices(nullptr)
	, mIndices(nullptr)
	, mNumVertices(0)
	, mNumIndices(0)
{
}

Mesh::~Mesh()
{
}

void Mesh::Allocate(uint32_t numVertices, uint32_t numIndices)
{
	mNumVertices = numVertices;
	mNumIndices = numIndices;

	SafeDeleteArray(mIndices);
	mIndices = new uint32_t[numIndices];
	SafeDeleteArray(mVertices);
	mVertices = new Vertex[numVertices];
}

void Mesh::Destroy()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
}
