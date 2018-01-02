#ifndef INCLUDED_GRAPHICS_VERTEXTYPES_H
#define INCLUDED_GRAPHICS_VERTEXTYPES_H

namespace Graphics
{

#define VF_POSITION 0X01 << 0
#define VF_NORMAL	0X01 << 1
#define VF_UV		0X01 << 2
#define VF_COLOR	0X01 << 3
#define VF_TANGENT	0x01 << 4
#define VF_BINDEX	0x01 << 5
#define VF_BWEIGHT	0x01 << 6

struct VertexP
{
	static const uint32_t Format = VF_POSITION;
	Math::Vector3 position;
};

struct VertexPN
{
	static const uint32_t Format = VF_POSITION | VF_NORMAL;
	Math::Vector3 position;
	Math::Vector3 normal;
};

struct VertexPC
{
	static const uint32_t Format = VF_POSITION | VF_COLOR;
	Math::Vector3 position;
	Math::Vector4 color;
};

struct VertexPT
{
	static const uint32_t Format = VF_POSITION | VF_UV;
	Math::Vector3 position;
	Math::Vector2 uv;
};

struct VertexPNC
{
	static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_COLOR;
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector4 color;
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

struct VertexBone
{
	static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_COLOR | VF_UV | VF_BINDEX | VF_BWEIGHT;
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector4 color;
	Math::Vector2 uv;
	int boneIndex[4] = {};
	float boneWeight[4] = {};
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_VERTEXTYPES_H