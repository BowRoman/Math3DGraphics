#include "Precompiled.h"
#include "PixelShader.h"
#include "GraphicsSystem.h"



void Graphics::PixelShader::Initialize(const wchar_t* fileName)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* ShaderBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile
	(
		fileName,
		nullptr,
		nullptr,
		"PS",
		"ps_5_0",
		shaderFlags,
		0,
		&ShaderBlob,
		&ErrorBlob
	);
	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error:%s", (char*)ErrorBlob->GetBufferPointer());
	SafeRelease(ErrorBlob);

	Graphics::GraphicsSystem::Get()->GetDevice()->CreatePixelShader
	(
		ShaderBlob->GetBufferPointer(),
		ShaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);

	SafeRelease(ShaderBlob);
}

void Graphics::PixelShader::Bind()
{
	Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();
	ID3D11DeviceContext* context = gs->GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}

void Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}
