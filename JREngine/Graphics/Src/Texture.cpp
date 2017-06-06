#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK\Inc\DDSTextureLoader.h>
#include <DirectXTK\Inc\WICTextureLoader.h>

using namespace Graphics;

Texture::Texture()
	: mShaderResourceView(nullptr)
{
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Sampler] Texture not released.");
}

void Texture::Initialize(const wchar_t * filename)
{
	ID3D11Device *device = GraphicsSystem::Get()->GetDevice();
	ID3D11DeviceContext *context = GraphicsSystem::Get()->GetContext();

	if (wcsstr(filename, L".dds") != nullptr)
	{
		DirectX::CreateDDSTextureFromFile(device, context, filename, nullptr, &mShaderResourceView);
	}
	else
	{
		DirectX::CreateWICTextureFromFile(device, context, filename, nullptr, &mShaderResourceView);
	}
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot)
{
	// Tell the shader which stage to apply at
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}
