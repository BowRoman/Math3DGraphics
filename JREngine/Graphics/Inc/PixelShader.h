#ifndef INCLUDED_GRAPHICS_PIXELSHADER_H
#define INCLUDED_GRAPHICS_PIXELSHADER_H

namespace Graphics
{

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

	void Initialize(const wchar_t* shaderLoc);
	void Terminate();
	void Bind();

private:
	ID3D11PixelShader* mPixelShader;
}; // class PixelShader

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_PIXELSHADER_H