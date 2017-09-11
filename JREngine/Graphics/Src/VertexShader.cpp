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
		if (vertexFormat & VF_UV)
		{
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VF_COLOR)
		{
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		return desc;
	}
}

VertexShader::VertexShader()
	: mInputLayout(nullptr)
	, mVertexShader(nullptr)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Initialize(const wchar_t* shaderFileLoc, uint32_t vertexFormat)
{
	// create vertex shader
	// D3DCOMPILE_DEBUG flag improves shader debugging
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile
	(
		shaderFileLoc,
		nullptr,
		nullptr,
		"VS",
		"vs_5_0",
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);
	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	GraphicsSystem::Get()->GetDevice()->CreateVertexShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);

	// define vertex description, create input layout using shader blob
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc = GetVertexDescription(vertexFormat);

	// create input layout
	GraphicsSystem::Get()->GetDevice()->CreateInputLayout
	(
		vertexDesc.data(),
		vertexDesc.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	SafeRelease(shaderBlob);
}

void VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
}

void VertexShader::Bind()
{
	GraphicsSystem::Get()->GetContext()->IASetInputLayout(mInputLayout);
	GraphicsSystem::Get()->GetContext()->VSSetShader(mVertexShader, nullptr, 0);
}
