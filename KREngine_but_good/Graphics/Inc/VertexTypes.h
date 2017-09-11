#pragma once

#define VF_POSITION 0x01 << 0
#define VF_NORMAL	0x01 << 1
#define VF_UV		0x01 << 2
#define VF_COLOR	0x01 << 3
#define VF_TANGENT	0x01 << 4



namespace Graphics
{
	struct VertexP
	{
		static const uint32_t Format = VF_POSITION;
		Math::Vector3 position;
	};

	struct VertexPC
	{
		static const uint32_t Format = VF_POSITION | VF_COLOR;
		Math::Vector3 position;
		Math::Vector4 color;
	};

	struct VertexPNC
	{
		static const uint32_t Format = VF_POSITION  | VF_NORMAL | VF_COLOR;
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector4 color;
	};

	struct VertexPT
	{
		static const uint32_t Format = VF_POSITION | VF_UV;
		Math::Vector3 position;
		Math::Vector2 uv;
	};

	struct VertexPNT
	{
		static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_UV;
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector2 uv;
	};

	struct VertexPNTT
	{
		static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_UV;
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector3 tangent;
		Math::Vector2 uv;
	};

	struct Vertex
	{
		static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_COLOR | VF_UV;
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector3 tangent;
		Math::Vector4 color;
		Math::Vector2 uv;
	};

}