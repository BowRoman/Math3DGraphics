#include "Precompiled.h"
#include "MeshBuffer.h"

#include "GraphicsSystem.h"

Graphics::MeshBuffer::MeshBuffer()
	: mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mVertexSize(0)
	, mVertexCapacity(0)
	, mVertexCount(0)
	, mIndexCount(0)
	, mTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

Graphics::MeshBuffer::~MeshBuffer()
{
}

// Initialize for use with vertex buffer
void Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;

	//create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = mVertexSize * mVertexCount; // memory size in VRAM
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mVertexBuffer);
}

// Initialize for use with index buffer
void Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, const uint32_t indexCount)
{
	Initialize(vertices, vertexSize, vertexCount);

	mIndexCount = indexCount;

	//create index buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t) * mIndexCount; // memory size in VRAM
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices;

	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mIndexBuffer);
}

void Graphics::MeshBuffer::InitializeDynamic(uint32_t vertexSize, uint32_t vertexCapacity)
{
	mVertexSize = vertexSize;
	mVertexCount = 0;
	mVertexCapacity = vertexCapacity;

	//create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = vertexSize * vertexCapacity; // memory size in VRAM
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, nullptr, &mVertexBuffer);
}

void Graphics::MeshBuffer::Terminate()
{
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);
}

void Graphics::MeshBuffer::SetVertexBuffer(const void * vertices, uint32_t vertexCount)
{
	ASSERT(vertices != nullptr && vertexCount > 0, "[MeshBuffer] Invalid parameters.");
	ASSERT(mVertexCapacity > vertexCount, "[MeshBuffer] Vertex count exceeds capacity.");

	ID3D11DeviceContext *context = Graphics::GraphicsSystem::Get()->GetContext();

	mVertexCount = vertexCount;

	if (vertexCount > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, vertices, vertexCount * mVertexSize);
		context->Unmap(mVertexBuffer, 0);
	}
}

void Graphics::MeshBuffer::SetToplogy(Topology topology)
{
	switch (topology)
	{
	case Topology::PointList:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Topology::LineList:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Topology::LineStrip:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		break;
	case Topology::TriangleList:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case Topology::TriangleStrip:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	default:
		ASSERT(false, "[MeshBuffer] Unexpected topology.");
		break;
	}
}

void Graphics::MeshBuffer::Render()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	// set vertex buffer
	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// set primitive topology
	context->IASetPrimitiveTopology(mTopology);

	// draw mesh
	if (mIndexCount == 0)
	{
		// using vertex buffer only
		context->Draw(mVertexCount, 0);
	}
	else
	{
		// using index buffer
		// set index buffer
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
}
