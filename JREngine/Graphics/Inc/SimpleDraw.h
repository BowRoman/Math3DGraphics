#ifndef INCLUDED_GRAPHICS_SIMPLEDRAW_H
#define INCLUDED_GRAPHICS_SIMPLEDRAW_H

namespace Graphics{
namespace SimpleDraw{

void StaticInitialize(uint32_t capacity);
void StaticTerminate();

void DrawLine(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector4& color);
void Flush(const Math::Matrix4& matViewProj);

} // namespace SimpleDraw
} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_SIMPLEDRAW_H