#include "Precompiled.h"
#include "GraphicsSystem.h"
#include "Texture.h"

#include <DirectXTK\Inc\DDSTextureLoader.h>
#include <DirectXTK\Inc\WICTextureLoader.h>

using namespace Graphics;

Graphics::Texture::Texture()
	:mShaderResourceView(nullptr)
{
	
}

Graphics::Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Texture] Texture not released");
}

void Graphics::Texture::Initialize(const wchar_t* fileName)
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();

	if (wcsstr(fileName, L".dds") != nullptr)
	{
		DirectX::CreateDDSTextureFromFile(device, context, fileName, nullptr, &mShaderResourceView);
	}
	else
	{
		DirectX::CreateWICTextureFromFile(device, context, fileName, nullptr, &mShaderResourceView);
	}
}

void Graphics::Texture::Initialize(const char* fileName)
{
	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, fileName, 1024);
	Initialize(wbuffer);
}

void Graphics::Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Graphics::Texture::BindVS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(slot,1, &mShaderResourceView);
}

void Graphics::Texture::BindPS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot,1, &mShaderResourceView);
}
