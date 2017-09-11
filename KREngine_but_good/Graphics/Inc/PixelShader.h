#pragma once



namespace Graphics
{
	class PixelShader
	{
	public:
		PixelShader() : mPixelShader(nullptr) {}
		~PixelShader() {};
		void Initialize(const wchar_t* fileName);
		void Bind();
		void Terminate();
	private:
		ID3D11PixelShader* mPixelShader;

	};
}