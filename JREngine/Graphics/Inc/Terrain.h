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

	void Initialize(const char* rawFileName, uint32_t columns, uint32_t rows, float scale = 1.f, float maxHeight = 10.f, Math::Vector3 origin = Math::Vector3::Zero());
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

	float* mHeightVertices;
	uint32_t mColumns;
	uint32_t mRows;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_TERRAIN_H