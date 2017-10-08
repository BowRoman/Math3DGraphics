//====================================================================================================
// Filename:	Texture.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_TEXTURE_H
#define INCLUDED_XENGINE_TEXTURE_H

namespace X {

class Texture
{
public:
	Texture();
	~Texture();
	
	bool Initialize(const char* filename);
	bool Initialize(const void* data, uint32_t width, uint32_t height);
	void Terminate();
	
	void BindVS(uint32_t index);
	void BindPS(uint32_t index);

private:
	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mShaderResourceView;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_TEXTURE_H