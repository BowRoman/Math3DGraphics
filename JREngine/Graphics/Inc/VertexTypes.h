#ifndef INCLUDED_GRAPHICS_VERTEXTYPES_H
#define INCLUDED_GRAPHICS_VERTEXTYPES_H

namespace Graphics
{

#define VF_POSITION 0X1 << 0
#define VF_NORMAL	0X1 << 1
#define VF_TANGENT	0X1 << 2
#define VF_UV		0X1 << 3
#define VF_COLOR	0X1 << 4

struct VertexP
{
	static const uint32_t format = VF_POSITION;
	Math::Vector3 position;
};

struct VertexPN
{
	static const uint32_t format = VF_POSITION | VF_NORMAL;
	Math::Vector3 position;
	Math::Vector3 normal;
};

struct VertexPC
{
	static const uint32_t format = VF_POSITION | VF_COLOR;
	Math::Vector3 position;
	Math::Vector4 color;
};

struct VertexPT
{
	static const uint32_t format = VF_POSITION | VF_UV;
	Math::Vector3 position;
	Math::Vector2 uv;
};

struct VertexPNC
{
	static const uint32_t format = VF_POSITION | VF_NORMAL | VF_COLOR;
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector4 color;
};

struct Vertex
{
	static const uint32_t format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_COLOR | VF_UV;
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector4 color;
	Math::Vector2 uv;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_VERTEXTYPES_H