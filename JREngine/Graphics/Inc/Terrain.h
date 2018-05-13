#ifndef INCLUDED_GRAPHICS_TERRAIN_H
#define INCLUDED_GRAPHICS_TERRAIN_H

#include "Texture.h"
#include "Sampler.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"

namespace Graphics
{

class Terrain
{
public:

	Terrain();
	~Terrain();

	void Initialize(const char* rawFileName, uint32_t cols, uint32_t rows);
	void Terminate();

	float GetHeight(uint32_t row, uint32_t col) const;
	uint32_t GetColumns() const;
	uint32_t GetRows() const;
	Mesh& GetMesh();

	void Render();

private:
	void GenerateIndices();

private:
	Mesh mMesh;
	MeshBuffer mMeshBuffer;

	uint32_t mNumHeightVertices;
	char* mHeightVertices;
	uint32_t mColumns;
	uint32_t mRows;

	float mScale = 1.0f;
	float mMaxHeight = 100.0f;
	Math::Vector3 origin = Math::Vector3::Zero();
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_TERRAIN_H