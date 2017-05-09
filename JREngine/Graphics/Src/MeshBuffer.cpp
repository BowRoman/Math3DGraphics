#include "Precompiled.h"
#include "MeshBuffer.h"

#include "GraphicsSystem.h"

Graphics::MeshBuffer::MeshBuffer()
	: mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mVertexSize(0)
	, mVertexCount(0)
	, mIndexCount(0)
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

void Graphics::MeshBuffer::Terminate()
{
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);
}

void Graphics::MeshBuffer::Render()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	// set vertex buffer
	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// set primitive topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
