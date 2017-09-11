#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"
#include <assert.h>

using namespace Graphics;


void Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;
	mVertexCapacity = vertexCount;

	//Vertex
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexSize * vertexCount; //memory size in VRAM
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;



	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	//Allocate 
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mVertexBuffer);
}

void Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const void* indices, const uint32_t indexCount)
{
	Initialize(vertices, vertexSize, vertexCount);
	mIndexCount = indexCount;

	//Index
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t) * indexCount; //memory size in VRAM
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices;

	//Allocate 
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mIndexBuffer);
}

void Graphics::MeshBuffer::InitializeDynamic(uint32_t VertexSize, uint32_t vertexCapacity)
{
	mVertexSize = VertexSize;
	mVertexCount = 0;
	mVertexCapacity = vertexCapacity;

	//Vertex
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = VertexSize * vertexCapacity; //memory size in VRAM
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	//Allocate 
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, nullptr, &mVertexBuffer);
}

void Graphics::MeshBuffer::SetVertexBuffer(const void* vertices, uint32_t vertexCount)
{
	ASSERT(vertices != nullptr, "[MeshBuffer] Invalid Parameters"); //removed check for vertexCount > 0
	ASSERT(vertexCount < mVertexCapacity, "[MeshBuffer] Too many vertices");
	mVertexCount = vertexCount;

	if (vertexCount > 0)
	{
		ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, vertices, vertexCount* mVertexSize);
		context->Unmap(mVertexBuffer, 0);

	}

}

void Graphics::MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::PointList:
		mTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Topology::LineList:
		mTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Topology::LineStrip:
		mTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		break;
	case Topology::TriangleList:
		mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case Topology::TriangleStrip:
		mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	default:
		ASSERT(false, "[MeshBuffer] Use a valid topology");
		break;
	}
}

void Graphics::MeshBuffer::Render()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	UINT stride = mVertexSize;
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(mTopology);

	if (mIndexBuffer == nullptr)
	{
		context->Draw(mVertexCount, 0);
	}
	else
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
}

void Graphics::MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

