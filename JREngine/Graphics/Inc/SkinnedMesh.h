#ifndef INCLUDED_GRAPHICS_SKINNEDMESH_H
#define INCLUDED_GRAPHICS_SKINNEDMESH_H

#include "VertexTypes.h"

namespace Graphics
{

class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

	void Allocate(uint32_t vertcount, uint32_t intcount);
	void Terminate();
	
	const VertexBone* GetVertices() const		{ return mVertices; }
	const uint32_t* GetIndices() const			{ return mIndices; }
	VertexBone& GetVertex(uint32_t index) const { return mVertices[index]; }

	uint32_t GetVertexCount() const			{ return mNumVertices; }
	uint32_t GetIndexCount() const			{ return mNumIndices; }

public:
	VertexBone* mVertices;
	uint32_t* mIndices;

	uint32_t mNumVertices;
	uint32_t mNumIndices;

}; // class SkinnedMesh

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_SKINNEDMESH_H