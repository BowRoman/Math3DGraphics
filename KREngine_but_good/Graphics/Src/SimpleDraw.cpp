#include "Precompiled.h"
#include "..\Inc\SimpleDraw.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexTypes.h"
#include "VertexShader.h"


using namespace Graphics;
using namespace Graphics::SimpleDraw;

namespace
{
	const char* kSimpleShader =
		"cbuffer CBSimpleDraw : register(b0)"
		"{"
		"	matrix transform;"
		"}"
		"struct A2V"
		"{"
		"	float4 position : POSITION;"
		"	float4 color : COLOR;"
		"};"
		"struct V2P"
		"{"
		"	float4 position : SV_POSITION;"
		"	float4 color : COLOR;"
		"};"
		"V2P VS(A2V input)"
		"{"
		"	V2P output = (V2P)0;"
		"	output.position = mul(input.position, transform);"
		"	output.color = input.color;"
		"	return output;"
		"}"
		"float4 PS(V2P input) : SV_Target"
		"{"
		"	return input.color;"
		"}";
	struct CBSimpleDraw
	{
		Math::Matrix4 transform;
	};


	class SimpleDrawImpl
	{
		struct ConstantData
		{
			Math::Matrix4 vp;
		};

		TypedConstantBuffer<ConstantData> mConstantBuffer;
		MeshBuffer mMeshBuffer;
		MeshBuffer mMeshBuffer2D;

		PixelShader mPixelShader;
		VertexShader mVertexShader;

		VertexPC* mVertices;
		VertexPC* mVertices2D;

		uint32_t mVertexCount;
		uint32_t mVertexCount2D;

		uint32_t mCapacity;

	public:
		SimpleDrawImpl()
			: mVertices(nullptr)
			, mVertices2D(nullptr)
			, mVertexCount(0)
			, mVertexCount2D(0)
			, mCapacity(0) {}

		~SimpleDrawImpl()
		{
			ASSERT(mVertices == nullptr, "[SimpleDraw] Memory not cleaned up");
		}
		void Initialize(int capacity)
		{
			mVertices = new VertexPC[capacity];
			mVertices2D = new VertexPC[capacity];
			mCapacity = capacity;
			mConstantBuffer.Initialize();
			mVertexShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
			mPixelShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx");
			mMeshBuffer.InitializeDynamic(sizeof(VertexPC), mCapacity);
			mMeshBuffer2D.InitializeDynamic(sizeof(VertexPC), mCapacity);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::LineList);
		}
		void Terminate()
		{
			mMeshBuffer.Terminate();
			mMeshBuffer2D.Terminate();
			mPixelShader.Terminate();
			mVertexShader.Terminate();
			mConstantBuffer.Terminate();

			SafeDelete(mVertices);
			SafeDelete(mVertices2D);

		}
		void DrawLine(const Math::Vector3 & p0, const Math::Vector3 & p1, const Math::Vector4 & color)
		{
			if (mVertexCount + 2 < mCapacity)
			{
				mVertices[mVertexCount++] = { p0, color };
				mVertices[mVertexCount++] = { p1, color };
			}
		}
		void Flush(const Math::Matrix4 & matViewProj)
		{
			ConstantData data;
			data.vp = Math::Matrix4();
			data.vp = Math::Transpose(matViewProj);
			mConstantBuffer.Set(data);
			mConstantBuffer.BindVS(); //matrix to vertex shader

			mVertexShader.Bind();
			mPixelShader.Bind();
			mMeshBuffer.SetVertexBuffer(mVertices, mVertexCount);
			mMeshBuffer2D.SetVertexBuffer(mVertices2D, mVertexCount2D);
			mMeshBuffer.Render();
			mMeshBuffer2D.Render();
		}

		// Functions to add 3D lines
		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color);
		void AddAABB(const Math::AABB& aabb, const Math::Vector4& color);
		void AddOBB(const Math::OBB& obb, const Math::Vector4& color);
		void AddSphere(const Math::Sphere& sphere, const Math::Vector4& color, uint32_t slices, uint32_t rings);
		void AddTransform(const Math::Matrix4& transform);

		// Functions to add screen lines
		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color);
		void AddScreenRect(const Math::Rect& rect, const Math::Vector4& color);
		void AddScreenCircle(const Math::Circle& circle, const Math::Vector4& color);
	};

	SimpleDrawImpl* sSimpleDraw = nullptr;
}


void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	// Check if we have enough space
	if (mVertexCount + 2 <= mCapacity)
	{
		// Add line
		mVertices[mVertexCount++] = { v0, color };
		mVertices[mVertexCount++] = { v1, color };
	}

	ASSERT(mVertexCount < mCapacity, "[SimpleDraw] Too many vertices!");
}

void SimpleDrawImpl::AddAABB(const Math::AABB& aabb, const Math::Vector4& color)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	// Check if we have enough space
	if (mVertexCount + 24 <= mCapacity)
	{
		float minX = aabb.center.x - aabb.extend.x;
		float minY = aabb.center.y - aabb.extend.y;
		float minZ = aabb.center.z - aabb.extend.z;
		float maxX = aabb.center.x + aabb.extend.x;
		float maxY = aabb.center.y + aabb.extend.y;
		float maxZ = aabb.center.z + aabb.extend.z;

		// Add lines
		mVertices[mVertexCount++] = { Math::Vector3(minX, minY, minZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(minX, minY, maxZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(minX, minY, maxZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(maxX, minY, maxZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(maxX, minY, maxZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(maxX, minY, minZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(maxX, minY, minZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(minX, minY, minZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(minX, minY, minZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(minX, maxY, minZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(minX, minY, maxZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(minX, maxY, maxZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(maxX, minY, maxZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, maxZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(maxX, minY, minZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, minZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(minX, maxY, minZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(minX, maxY, maxZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(minX, maxY, maxZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, maxZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, maxZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, minZ), color };

		mVertices[mVertexCount++] = { Math::Vector3(maxX, maxY, minZ), color };
		mVertices[mVertexCount++] = { Math::Vector3(minX, maxY, minZ), color };
	}

	ASSERT(mVertexCount < mCapacity, "[SimpleDraw] Too many vertices!");
}

void SimpleDrawImpl::AddOBB(const Math::OBB& obb, const Math::Vector4& color)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	Math::Matrix4 toWorld = matScale * matRot * matTrans;

	Math::Vector3 points[] =
	{
		Math::Vector3(-1.0f, -1.0f, -1.0f),
		Math::Vector3(-1.0f,  1.0f, -1.0f),
		Math::Vector3(1.0f,  1.0f, -1.0f),
		Math::Vector3(1.0f, -1.0f, -1.0f),
		Math::Vector3(-1.0f, -1.0f,  1.0f),
		Math::Vector3(-1.0f,  1.0f,  1.0f),
		Math::Vector3(1.0f,  1.0f,  1.0f),
		Math::Vector3(1.0f, -1.0f,  1.0f)
	};

	for (uint32_t i = 0; i < 8; ++i)
	{
		points[i] = Math::TransformCoord(points[i], toWorld);
	}

	AddLine(points[0], points[1], color);
	AddLine(points[1], points[2], color);
	AddLine(points[2], points[3], color);
	AddLine(points[3], points[0], color);

	AddLine(points[0], points[4], color);
	AddLine(points[1], points[5], color);
	AddLine(points[2], points[6], color);
	AddLine(points[3], points[7], color);

	AddLine(points[4], points[5], color);
	AddLine(points[5], points[6], color);
	AddLine(points[6], points[7], color);
	AddLine(points[7], points[4], color);
}

void SimpleDrawImpl::AddSphere(const Math::Sphere& sphere, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	const float x = sphere.center.x;
	const float y = sphere.center.y;
	const float z = sphere.center.z;
	const float radius = sphere.radius;

	const uint32_t kSlices = Math::Max(3u, slices);
	const uint32_t kRings = Math::Max(2u, rings);
	const uint32_t kLines = (4 * kSlices * kRings) - (2 * kSlices);

	// Check if we have enough space
	if (mVertexCount + kLines <= mCapacity)
	{
		// Add lines
		const float kTheta = Math::kPi / (float)kRings;
		const float kPhi = Math::kTwoPi / (float)kSlices;
		for (uint32_t j = 0; j < kSlices; ++j)
		{
			for (uint32_t i = 0; i < kRings; ++i)
			{
				const float a = i * kTheta;
				const float b = a + kTheta;
				const float ay = radius * cos(a);
				const float by = radius * cos(b);

				const float theta = j * kPhi;
				const float phi = theta + kPhi;

				const float ar = sqrt(radius * radius - ay * ay);
				const float br = sqrt(radius * radius - by * by);

				const float x0 = x + (ar * sin(theta));
				const float y0 = y + (ay);
				const float z0 = z + (ar * cos(theta));

				const float x1 = x + (br * sin(theta));
				const float y1 = y + (by);
				const float z1 = z + (br * cos(theta));

				const float x2 = x + (br * sin(phi));
				const float y2 = y + (by);
				const float z2 = z + (br * cos(phi));

				mVertices[mVertexCount++] = { Math::Vector3(x0, y0, z0), color };
				mVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };

				if (i < kRings - 1)
				{
					mVertices[mVertexCount++] = { Math::Vector3(x1, y1, z1), color };
					mVertices[mVertexCount++] = { Math::Vector3(x2, y2, z2), color };
				}
			}
		}
	}

	ASSERT(mVertexCount < mCapacity, "[SimpleDraw] Too many vertices!");
}

void SimpleDrawImpl::AddTransform(const Math::Matrix4& transform)
{
	Math::Vector3 position = Math::GetTranslation(transform);
	Math::Vector3 right = Math::GetRight(transform);
	Math::Vector3 up = Math::GetUp(transform);
	Math::Vector3 forward = Math::GetForward(transform);
	AddLine(position, position + right, Math::Vector4::Red());
	AddLine(position, position + up, Math::Vector4::Green());
	AddLine(position, position + forward, Math::Vector4::Blue());
}

void SimpleDrawImpl::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	// Check if we have enough space
	if (mVertexCount2D + 2 <= mCapacity)
	{
		// Add line
		mVertices2D[mVertexCount2D++] = { Math::Vector3(v0.x, v0.y, 0.0f), color };
		mVertices2D[mVertexCount2D++] = { Math::Vector3(v1.x, v1.y, 0.0f), color };
	}

	ASSERT(mVertexCount2D < mCapacity, "[SimpleDraw] Too many vertices!");
}

void SimpleDrawImpl::AddScreenRect(const Math::Rect& rect, const Math::Vector4& color)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	// Check if we have enough space
	if (mVertexCount2D + 8 <= mCapacity)
	{
		float l = rect.left;
		float t = rect.top;
		float r = rect.right;
		float b = rect.bottom;

		// Add lines
		mVertices2D[mVertexCount2D++] = { Math::Vector3(l, t, 0.0f), color };
		mVertices2D[mVertexCount2D++] = { Math::Vector3(r, t, 0.0f), color };

		mVertices2D[mVertexCount2D++] = { Math::Vector3(r, t, 0.0f), color };
		mVertices2D[mVertexCount2D++] = { Math::Vector3(r, b, 0.0f), color };

		mVertices2D[mVertexCount2D++] = { Math::Vector3(r, b, 0.0f), color };
		mVertices2D[mVertexCount2D++] = { Math::Vector3(l, b, 0.0f), color };

		mVertices2D[mVertexCount2D++] = { Math::Vector3(l, b, 0.0f), color };
		mVertices2D[mVertexCount2D++] = { Math::Vector3(l, t, 0.0f), color };
	}

	ASSERT(mVertexCount2D < mCapacity, "[SimpleDraw] Too many vertices!");
}

void SimpleDrawImpl::AddScreenCircle(const Math::Circle& circle, const Math::Vector4& color)
{
	//ASSERT(mInitialized, "[SimpleDraw] Not initialized.");

	// Check if we have enough space
	if (mVertexCount2D + 32 <= mCapacity)
	{
		float x = circle.center.x;
		float y = circle.center.y;
		float r = circle.radius;

		// Add line
		const float kAngle = Math::kPi / 8.0f;
		for (uint32_t i = 0; i < 16; ++i)
		{
			const float alpha = i * kAngle;
			const float beta = alpha + kAngle;
			const float x0 = x + (r * sin(alpha));
			const float y0 = y + (r * cos(alpha));
			const float x1 = x + (r * sin(beta));
			const float y1 = y + (r * cos(beta));
			mVertices2D[mVertexCount2D++] = { Math::Vector3(x0, y0, 0.0f), color };
			mVertices2D[mVertexCount2D++] = { Math::Vector3(x1, y1, 0.0f), color };
		}
	}

	ASSERT(mVertexCount2D < mCapacity, "[SimpleDraw] Too many vertices!");
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddLine(v0, v1, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Math::Vector4& color)
{
	AddLine(Math::Vector3(x0, y0, z0), Math::Vector3(x1, y1, z1), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(const Math::AABB& aabb, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddAABB(aabb, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Math::Vector4& color)
{
	AddAABB(Math::AABB((min + max) * 0.5f, (max - min) * 0.5f), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(const Math::Vector3& center, float radius, const Math::Vector4& color)
{
	AddAABB(Math::AABB(center, Math::Vector3(radius, radius, radius)), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Math::Vector4& color)
{
	AddAABB(Math::Vector3(minX, minY, minZ), Math::Vector3(maxX, maxY, maxZ), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddOBB(const Math::OBB& obb, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddOBB(obb, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddSphere(const Math::Sphere& sphere, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddSphere(sphere, color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	AddSphere(Math::Sphere(center, radius), color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddSphere(float x, float y, float z, float radius, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	AddSphere(Math::Sphere(x, y, z, radius), color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddTransform(transform);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddScreenLine(v0, v1, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenLine(float x0, float y0, float x1, float y1, const Math::Vector4& color)
{
	AddScreenLine(Math::Vector2(x0, y0), Math::Vector2(x1, y1), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenRect(const Math::Rect& rect, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddScreenRect(rect, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Math::Vector4& color)
{
	AddScreenRect(Math::Rect(min.x, min.y, max.x, max.y), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenRect(float left, float top, float right, float bottom, const Math::Vector4& color)
{
	AddScreenRect(Math::Rect(left, top, right, bottom), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenCircle(const Math::Circle& circle, const Math::Vector4& color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDraw->AddScreenCircle(circle, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenCircle(const Math::Vector2& center, float r, const Math::Vector4& color)
{
	AddScreenCircle(Math::Circle(center, r), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenCircle(float x, float y, float r, const Math::Vector4& color)
{
	AddScreenCircle(Math::Circle(x, y, r), color);
}

//----------------------------------------------------------------------------------------------------



void SimpleDraw::StaticInitialize(uint32_t capacity)
{
	ASSERT(sSimpleDraw == nullptr, "[SimpleDraw] already initialized");
	sSimpleDraw = new SimpleDrawImpl();
	sSimpleDraw->Initialize(capacity);
}

void SimpleDraw::StaticTerminate()
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] not initialized");
	sSimpleDraw->Terminate();
	SafeDelete(sSimpleDraw);
}

void SimpleDraw::DrawLine(const Math::Vector3 & p0, const Math::Vector3 & p1, const Math::Vector4 & color)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] not initialized");
	sSimpleDraw->DrawLine(p0, p1, color);
}

void SimpleDraw::Flush(const Math::Matrix4& matViewProj)
{
	ASSERT(sSimpleDraw != nullptr, "[SimpleDraw] not initialized");
	sSimpleDraw->Flush(matViewProj);
}
