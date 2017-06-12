#include "Precompiled.h"
#include "MeshBuilder.h"

#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace Graphics;

namespace
{

class MeshBuilderImpl
{
public:
	MeshBuilderImpl();
	~MeshBuilderImpl();

	void Initialize(uint32_t capacity, MeshBuffer::Topology topology = MeshBuffer::Topology:::t)
	{

		mConstantBuffer.Initialize();
		//mMeshBuffer.Initialize(kVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);
		mVertexShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx", VertexPC::format);
		mPixelShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx");
		mMeshBuffer.InitializeDynamic(sizeof(VertexPC), capacity);
		mMeshBuffer.SetToplogy(topology);
	}
	void Terminate()
	{
		mPixelShader.Terminate();
		mVertexShader.Terminate();
		mConstantBuffer.Terminate();

		SafeDeleteArray(mVertices);
	}

private:
	struct ConstantData
	{
		Math::Matrix4 vp;
	};

	TypedConstantBuffer<ConstantData> mConstantBuffer;
	MeshBuffer mMeshBuffer;
	PixelShader mPixelShader;
	VertexShader mVertexShader;

	VertexPC *mVertices;
	uint32_t mVertexCount;
	uint32_t mCapacity;

}; // class MeshBuilder

}