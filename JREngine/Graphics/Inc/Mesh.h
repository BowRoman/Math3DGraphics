#ifndef INCLUDED_GRAPHICS_MESH_H
#define INCLUDED_GRAPHICS_MESH_H

#include "VertexTypes.h"

namespace Graphics
{

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Allocate(uint32_t numVertices, uint32_t numIndices);
	void Destroy();

	const Vertex* GetVertices() const		{ return mVertices; }
	const uint32_t* GetIndices() const		{ return mIndices; }
	Vertex& GetVertex(uint32_t index) const { return mVertices[index]; }

	uint32_t GetVertexCount() const			{ return mNumVertices; }
	uint32_t GetIndexCount() const			{ return mNumIndices; }

private:
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	friend class MeshBuilder;
	friend class Terrain;

public:
	Vertex* mVertices;
	uint32_t* mIndices;

	uint32_t mNumVertices;
	uint32_t mNumIndices;

}; // class Mesh

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_MESH_H