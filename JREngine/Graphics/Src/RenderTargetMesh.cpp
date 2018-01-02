#include "Precompiled.h"
#include "RenderTargetMesh.h"
#include "GraphicsSystem.h"
#include <assert.h>



void Graphics::RenderTargetMesh::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	mVertexSize = vertexSize;
	mNumVertices = vertexCount;
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

void Graphics::RenderTargetMesh::InitializeDynamic(uint32_t VertexSize, uint32_t vertexCapacity)
{
	mVertexSize = VertexSize;
	mNumVertices = 0;
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

void Graphics::RenderTargetMesh::SetVertexBuffer(const void* vertices, uint32_t vertexCount)
{
	ASSERT(vertices != nullptr && vertexCount > 0, "[RenderTargetMesh] Invalid Parameters");
	ASSERT(vertexCount < mVertexCapacity, "[RenderTargetMesh] Too many vertices");
	mNumVertices = vertexCount;

	if (vertexCount > 0)
	{
		ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();
		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, vertices, vertexCount* mVertexSize);
		context->Unmap(mVertexBuffer, 0);

	}

}

void Graphics::RenderTargetMesh::SetTopology(Topology topology)
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
		ASSERT(false, "[RenderTargetMesh] Use a valid topology");
		break;
	}
}

void Graphics::RenderTargetMesh::Render()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	UINT stride = mVertexSize;
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(mTopology);

	context->Draw(mNumVertices, 0);

}

void Graphics::RenderTargetMesh::Terminate()
{
	SafeRelease(mVertexBuffer);
}

