#ifndef INCLUDED_GRAPHICS_VERTEXSHADER_H
#define INCLUDED_GRAPHICS_VERTEXSHADER_H

namespace Graphics
{

class VertexShader
{
public:
	VertexShader();
	~VertexShader();

	void Initialize(const wchar_t* shaderFileLoc, uint32_t vertexFormat);
	void Terminate();
	void Bind();

private:
	ID3D11InputLayout* mInputLayout;
	ID3D11VertexShader* mVertexShader;
};

}

#endif // #ifndef INCLUDED_GRAPHICS_VERTEXSHADER_H