#include "Precompiled.h"
#include "PixelShader.h"

#include "GraphicsSystem.h"

Graphics::PixelShader::PixelShader()
	: mPixelShader(nullptr)
{
}

Graphics::PixelShader::~PixelShader()
{
}

void Graphics::PixelShader::Initialize(const wchar_t* shaderFileLoc)
{
	// create pixel shader
	// D3DCOMPILE_DEBUG flag improves shader debugging
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile
	(
		shaderFileLoc,
		nullptr,
		nullptr,
		"PS",
		"ps_5_0",
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);
	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// create pixel buffer
	Graphics::GraphicsSystem::Get()->GetDevice()->CreatePixelShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	SafeRelease(shaderBlob);
}

void Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void Graphics::PixelShader::Bind()
{
	Graphics::GraphicsSystem::Get()->GetContext()->PSSetShader(mPixelShader, nullptr, 0);
}
