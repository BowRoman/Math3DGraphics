#ifndef INCLUDED_GRAPHICS_MESHBUILDER_H
#define INCLUDED_GRAPHICS_MESHBUILDER_H

#include "Mesh.h"

namespace Graphics {

class MeshBuilder
{
public:
	static void CreatePlane(Mesh& mesh, uint32_t rows, uint32_t columns, float scale);
	static void CreateSphere(Mesh& mesh, uint32_t slices, uint32_t rings);

}; // class MeshBuilder

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_MESHBUILDER_H