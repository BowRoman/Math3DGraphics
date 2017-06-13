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

	void Initialize(const char *rawFileName, uint32_t columns, uint32_t rows, float scale = 1.f, Math::Vector3 origin = Math::Vector3::Zero(), uint32_t distBtwnVert = 1);
	void Terminate();

	void Render();

private:
	Mesh mMesh;
	MeshBuffer mMeshBuffer;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_TERRAIN_H