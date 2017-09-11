#pragma once

namespace Graphics
{
	namespace SimpleDraw
	{
		void StaticInitialize(uint32_t capacity);
		void StaticTerminate();

		void DrawLine(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector4& color);
		void Flush(const Math::Matrix4& matViewProj);

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector4& color);
		void AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Math::Vector4& color);
		void AddAABB(const Math::AABB& aabb, const Math::Vector4& color);
		void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Math::Vector4& color);
		void AddAABB(const Math::Vector3& center, float radius, const Math::Vector4& color);
		void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Math::Vector4& color);
		void AddOBB(const Math::OBB& obb, const Math::Vector4& color);
		void AddSphere(const Math::Sphere& sphere, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
		void AddSphere(const Math::Vector3& center, float radius, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
		void AddSphere(float x, float y, float z, float radius, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
		void AddTransform(const Math::Matrix4& transform);

		// Functions for screen space rendering
		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color);
		void AddScreenLine(float x0, float y0, float x1, float y1, const Math::Vector4& color);
		void AddScreenRect(const Math::Rect& rect, const Math::Vector4& color);
		void AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Math::Vector4& color);
		void AddScreenRect(float left, float top, float right, float bottom, const Math::Vector4& color);
		void AddScreenCircle(const Math::Circle& circle, const Math::Vector4& color);
		void AddScreenCircle(const Math::Vector2& center, float r, const Math::Vector4& color);
		void AddScreenCircle(float x, float y, float r, const Math::Vector4& color);
	}
}