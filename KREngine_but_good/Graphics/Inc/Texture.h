#pragma once


namespace Graphics
{
	class Texture
	{

		ID3D11ShaderResourceView* mShaderResourceView;
	public:
		Texture();
		~Texture();

		void Initialize(const wchar_t* fileName);
		void Initialize(const char* fileName);
		void Terminate();

		void BindVS(uint32_t slot);
		void BindPS(uint32_t slot);

	};

}