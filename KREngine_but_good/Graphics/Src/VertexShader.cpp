#include "Precompiled.h"
#include "VertexShader.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace Graphics;

namespace
{

	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexDescription(uint32_t vertexFormat)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		if (vertexFormat & VF_POSITION)
		{
			desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VF_NORMAL)
		{
			desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VF_TANGENT)
		{
			desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VF_COLOR)
		{
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VF_UV)
		{
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		return desc;
	}

}


void Graphics::VertexShader::Initialize(const wchar_t* fileName, uint32_t vertexFormat)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* ShaderBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile
	(
		fileName,
		nullptr,
		nullptr,
		"VS",
		"vs_5_0",
		shaderFlags,
		0,
		&ShaderBlob,
		&ErrorBlob
	);
	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error:%s", (char*)ErrorBlob->GetBufferPointer());
	SafeRelease(ErrorBlob);

	Graphics::GraphicsSystem::Get()->GetDevice()->CreateVertexShader
	(
		ShaderBlob->GetBufferPointer(),
		ShaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc = GetVertexDescription(vertexFormat);
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateInputLayout
	(
		vertexDesc.data(),
		vertexDesc.size(),
		ShaderBlob->GetBufferPointer(),
		ShaderBlob->GetBufferSize(),
		&mInputLayout
	);
	SafeRelease(ShaderBlob);
}



void Graphics::VertexShader::Bind()
{
	Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();
	ID3D11DeviceContext* context = gs->GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
}



void Graphics::VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
}

