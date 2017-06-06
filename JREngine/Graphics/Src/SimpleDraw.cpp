#include "Precompiled.h"
#include "SimpleDraw.h"

#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace Graphics;

namespace {

class SimpleDrawImpl
{
public:
	SimpleDrawImpl()
		: mVertices(nullptr)
		, mVertexCount(0)
		, mCapacity(0)
	{}
	~SimpleDrawImpl()
	{
		ASSERT(mVertices == nullptr, "[SimpleDraw] Memory not cleared.");
	}

	//-------------------------------------------------------------------------------------------------------

	void Initialize(uint32_t capacity, MeshBuffer::Topology topology = MeshBuffer::Topology::LineList)
	{
		mVertices = new VertexPC[capacity];
		mCapacity = capacity;

		mConstantBuffer.Initialize();
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

	//-------------------------------------------------------------------------------------------------------

	void DrawLine(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector4& color)
	{
		if(mVertexCount + 2 < mCapacity)
		{
			mVertices[mVertexCount++] = { p0,color };
			mVertices[mVertexCount++] = { p1,color };
		}
	}
	void DrawSphere(const Math::Vector3& position, uint32_t slices, uint32_t rings, float scale, const Math::Vector4& color)
	{
		/*float incXZ = Math::kPi / slices;
		float incY = Math::kPi / rings;
		Math::Vector3 curPos(position.x - (Math::kPi / 2), position.y - (Math::kPi / 2), position.z - (Math::kPi / 2));
		if (mVertexCount + 2 < mCapacity)
		{
			for (float y = 0.f; y <= Math::kPi; y+=incY)
			{
				for (float xz = 0.f; xz <= Math::kPi; xz += incXZ)
				{

					mVertices[mVertexCount++] = { curPos,color };
				}
			}
		}*/
	}
	// Render vertices and reset the buffer
	void Flush(const Math::Matrix4& matViewProj)
	{
		ConstantData data;
		data.vp = Math::Transpose(matViewProj);
		mConstantBuffer.Set(data);
		mConstantBuffer.BindVS();

		// bind input layout, and vertex/pixel shaders
		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.SetVertexBuffer(mVertices,mVertexCount);
		mMeshBuffer.Render();

		// rewind vertex data
		mVertexCount = 0;
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
}; // class SimpleDraw

SimpleDrawImpl *sSimpleDraw = nullptr;

} // namespace

void SimpleDraw::StaticInitialize(uint32_t capacity)
{
	ASSERT(sSimpleDraw == nullptr, "[SimpleDraw] Already initialized.");
	sSimpleDraw = new SimpleDrawImpl();
	sSimpleDraw->Initialize(capacity);
}

void SimpleDraw::StaticTerminate()
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->Terminate();
}

void SimpleDraw::DrawLine(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->DrawLine(p0, p1, color);
}

void SimpleDraw::DrawSphere(const Math::Vector3 & position, uint32_t slices, uint32_t rings, float scale, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->DrawSphere(position, slices, rings, scale, color);}

void SimpleDraw::Flush(const Math::Matrix4& matViewProj)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->Flush(matViewProj);
}