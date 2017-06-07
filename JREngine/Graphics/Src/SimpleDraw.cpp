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
		mMeshBuffer.SetToplogy(MeshBuffer::Topology::LineList);
		if(mVertexCount + 2 < mCapacity)
		{
			mVertices[mVertexCount++] = { p0,color };
			mVertices[mVertexCount++] = { p1,color };
		}
	}
	void DrawSphere(const Math::Vector3& position, uint32_t slices, uint32_t rings, float scale, const Math::Vector4& color)
	{
		if (scale > 0 && slices > 0 && rings > 0)
		{
			mMeshBuffer.SetToplogy(MeshBuffer::Topology::LineStrip);
			if ((mVertexCount + (4*rings*slices)) <= mCapacity)
			{
				/*float x, y, z, radius;
				for (uint32_t i = 0; i <= rings; ++i)
				{
					for (uint32_t j = 0; j <= slices; ++j)
					{
						y = (-cos(Math::kPi * i / rings)*scale) + position.y;
						radius = sqrt(scale - (y*y));
						float xz = 2.0 * Math::kPi * j / slices;
						x = (radius * sin(xz)) + position.x;
						z = (radius * cos(xz)) + position.z;

						mVertices[mVertexCount++] = { Math::Vector3(x,y,z),color };
					}
				}*/
				const float kTheta = Math::kPi / (float)rings;
				const float kPhi = Math::kTwoPi / (float)slices;
				for (uint32_t j = 0; j < slices; ++j)
				{
					for (uint32_t i = 0; i < rings; ++i)
					{
						const float a = i * kTheta;
						const float b = a + kTheta;
						const float radius = scale / 2;
						const float ay = radius * cos(a);
						const float by = radius * cos(b);

						const float theta = j * kPhi;
						const float phi = theta + kPhi;

						const float ar = sqrt(radius * radius - ay * ay);
						const float br = sqrt(radius * radius - by * by);

						const float x0 = position.x + (ar * sin(theta));
						const float y0 = position.y + (ay);
						const float z0 = position.z + (ar * cos(theta));

						const float x1 = position.x + (br * sin(theta));
						const float y1 = position.y + (by);
						const float z1 = position.z + (br * cos(theta));

						const float x2 = position.x + (br * sin(phi));
						const float y2 = position.y + (by);
						const float z2 = position.z + (br * cos(phi));

						mVertices[mVertexCount++] = { Math::Vector3(x0, y0, z0), color };
						mVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };

						if (i < rings - 1)
						{
							mVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };
							mVertices[mVertexCount++] = { Math::Vector3(x2, y2, z2), color };
						}
					}
				}
			}
			else
			{
				mVertices[mVertexCount++] = { position,color };
			}
		}
		else
		{
			mVertices[mVertexCount++] = { position,color };
		}
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