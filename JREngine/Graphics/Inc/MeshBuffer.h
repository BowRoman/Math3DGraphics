#ifndef INCLUDED_GRAPHICS_MESHBUFFER_H
#define INCLUDED_GRAPHICS_MESHBUFFER_H

namespace Graphics
{

class MeshBuffer
{
public:
	MeshBuffer();
	~MeshBuffer();
	
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, const uint32_t indexCount);
	void Terminate();
	void Render();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	uint32_t mVertexSize;
	uint32_t mVertexCount;
	uint32_t mIndexCount;
};

}

#endif // #ifndef INCLUDED_GRAPHICS_MESHBUFFER_H