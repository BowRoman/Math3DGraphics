#pragma once
 
namespace Graphics
{
	class VertexShader
	{
	public:
		VertexShader() : mInputLayout(nullptr), mVertexShader(nullptr) {}
		~VertexShader() {};
		void Initialize(const wchar_t* fileName, uint32_t vertexFormat);
		void Bind();
		void Terminate();
	private:
		ID3D11InputLayout* mInputLayout;
		ID3D11VertexShader* mVertexShader;
	};
}