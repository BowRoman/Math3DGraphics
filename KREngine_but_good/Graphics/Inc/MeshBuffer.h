#pragma once

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
	MeshBuffer() : mVertexBuffer(nullptr), mIndexBuffer(nullptr) , mVertexSize(0), mVertexCount(0), mIndexCount(0), mVertexCapacity(0), mTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {}
	~MeshBuffer() {};
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const void* indices, const uint32_t indexCount);
	void InitializeDynamic(uint32_t VertexSize, uint32_t maxVertexCount);
	void SetVertexBuffer(const void* vertices, uint32_t vertexCount);
	void SetTopology(Topology topology);
	void Render();
	void Terminate();
private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	uint32_t mVertexSize;
	uint32_t mVertexCount;
	uint32_t mVertexCapacity;
	uint32_t mIndexCount;
	D3D10_PRIMITIVE_TOPOLOGY mTopology;
}; // class MeshBuffer

} // namespace Graphics
