#ifndef INCLUDED_GRAPHICS_SIMPLEDRAW_H
#define INCLUDED_GRAPHICS_SIMPLEDRAW_H

namespace Graphics
{

namespace SimpleDraw
{

void StaticInitialize(uint32_t capacity);
void StaticTerminate();

void DrawLine(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector4& color = Math::Vector4::White());
void DrawTransform(const Math::Matrix4& trans);
void DrawSphere(const Math::Vector3& position, uint32_t slices = 6, uint32_t rings = 6, float scale = 1.f, const Math::Vector4& color = Math::Vector4::White());
void Flush(const Math::Matrix4& matViewProj);

} // namespace SimpleDraw

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_SIMPLEDRAW_H