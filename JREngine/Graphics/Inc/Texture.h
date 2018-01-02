#ifndef INCLUDED_GRAPHICS_TEXTURE_H
#define INCLUDED_GRAPHICS_TEXTURE_H

namespace Graphics
{

class Texture
{
public:
	Texture();
	~Texture();

	void Initialize(const wchar_t* filename);
	void Initialize(const char* fileName);
	void Terminate();

	void BindVS(uint32_t slot);
	void BindPS(uint32_t slot);

private:
	ID3D11ShaderResourceView *mShaderResourceView;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_TEXTURE_H