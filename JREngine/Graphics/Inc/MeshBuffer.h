#ifndef INCLUDED_GRAPHICS_MESHBUFFER_H
#define INCLUDED_GRAPHICS_MESHBUFFER_H

namespace Graphics
{

class MeshBuffer
{
public:
	enum class Topology
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip
	};
	MeshBuffer();
	~MeshBuffer();
	
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, const uint32_t indexCount);
	void InitializeDynamic(uint32_t vertexSize, uint32_t vertexCapacity);
	void Terminate();

	void SetVertexBuffer(const void* vertices, uint32_t vertexCount);
	void SetToplogy(Topology topology);
	void Render();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	uint32_t mVertexSize;
	uint32_t mVertexCapacity;
	uint32_t mVertexCount;
	uint32_t mIndexCount;

	D3D11_PRIMITIVE_TOPOLOGY mTopology;
}; // class MeshBuffer

} //namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_MESHBUFFER_H