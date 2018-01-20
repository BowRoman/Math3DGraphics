#pragma once

namespace Graphics
{

class RenderTargetMesh
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

	RenderTargetMesh() : mVertexBuffer(nullptr), mVertexSize(0), mNumVertices(0),  mVertexCapacity(0), mTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP) {}
	~RenderTargetMesh() {};
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	void InitializeDynamic(uint32_t VertexSize, uint32_t maxVertexCount);
	void SetVertexBuffer(const void* vertices, uint32_t vertexCount);
	void SetTopology(Topology topology);
	void Render();
	void Terminate();

private:
	ID3D11Buffer* mVertexBuffer;
	uint32_t mVertexSize;
	uint32_t mNumVertices;
	uint32_t mVertexCapacity;
	D3D10_PRIMITIVE_TOPOLOGY mTopology;

}; // class RenderTargetMesh

} // namespace Graphics