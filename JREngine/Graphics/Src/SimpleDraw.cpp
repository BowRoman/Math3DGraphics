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
		mVertexShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx");
		mMeshBuffer.InitializeDynamic(sizeof(VertexPC), capacity);
		mMeshBuffer.SetTopology(topology);
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
		mMeshBuffer.SetTopology(MeshBuffer::Topology::LineList);
		if(mVertexCount + 2 < mCapacity)
		{
			mVertices[mVertexCount++] = { p0,color };
			mVertices[mVertexCount++] = { p1,color };
		}
	}
	void DrawTransform(const Math::Matrix4& trans)
	{
		Math::Vector3 pos(trans._41, trans._42, trans._43);
		Math::Vector3 xPos{ pos + Math::Vector3(trans._11, trans._12, trans._13) };
		Math::Vector3 yPos{ pos + Math::Vector3(trans._21, trans._22, trans._23) };
		Math::Vector3 zPos{ pos + Math::Vector3(trans._31, trans._32, trans._33) };
		DrawLine(pos, xPos, Math::Vector4::Red());
		DrawLine(pos, yPos, Math::Vector4::Green());
		DrawLine(pos, zPos, Math::Vector4::Blue());
	}
	void DrawSphere(const Math::Vector3& position, uint32_t slices, uint32_t rings, float scale, const Math::Vector4& color)
	{
		if (scale > 0 && slices > 0 && rings > 0)
		{
			mMeshBuffer.SetTopology(MeshBuffer::Topology::LineList);
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
						const float currRingAngle = i * kTheta;
						const float nextRingAngle = currRingAngle + kTheta;
						const float radius = scale / 2;
						const float currRingArmLength = radius * cos(currRingAngle);
						const float nextRingArmLength = radius * cos(nextRingAngle);

						const float theta = j * kPhi;
						const float phi = theta + kPhi;

						const float currRingOuterEdgeLength = sqrt(radius * radius - currRingArmLength * currRingArmLength);
						const float nextRingOuterEdgeLength = sqrt(radius * radius - nextRingArmLength * nextRingArmLength);

						const float x0 = position.x + (currRingOuterEdgeLength * sin(theta));
						const float y0 = position.y + (currRingArmLength);
						const float z0 = position.z + (currRingOuterEdgeLength * cos(theta));

						const float x1 = position.x + (nextRingOuterEdgeLength * sin(theta));
						const float y1 = position.y + (nextRingArmLength);
						const float z1 = position.z + (nextRingOuterEdgeLength * cos(theta));

						mVertices[mVertexCount++] = { Math::Vector3(x0, y0, z0), color };
						mVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };

						if (i < rings - 1)
						{
							const float x2 = position.x + (nextRingOuterEdgeLength * sin(phi));
							const float y2 = position.y + (nextRingArmLength);
							const float z2 = position.z + (nextRingOuterEdgeLength * cos(phi));

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

void SimpleDraw::DrawTransform(const Math::Matrix4& trans)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->DrawTransform(trans);
}

void SimpleDraw::DrawSphere(const Math::Vector3 & position, uint32_t slices, uint32_t rings, float scale, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->DrawSphere(position, slices, rings, scale, color);
}

void SimpleDraw::Flush(const Math::Matrix4& matViewProj)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] No instance exists.");
	sSimpleDraw->Flush(matViewProj);
}