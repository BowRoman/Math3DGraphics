//====================================================================================================
// Filename:	Texture.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/DDSTextureLoader.h>
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace X;

Texture::Texture()
	: mShaderResourceView(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

Texture::~Texture()
{
	XASSERT(mShaderResourceView == nullptr, "[Texture] Texture not released!");
}

//----------------------------------------------------------------------------------------------------

bool Texture::Initialize(const char* filename)
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, filename, 1024);

	if (strstr(filename, ".dds") != nullptr)
	{
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, wbuffer, nullptr, &mShaderResourceView);
		if (FAILED(hr))
		{
			XLOG("[Texture] Failed to load texture %s. HRESULT: 0x%x)", filename, hr);
			return false;
		}
	}
	else
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, wbuffer, nullptr, &mShaderResourceView);
		if (FAILED(hr))
		{
			XLOG("[Texture] Failed to load texture %s. HRESULT: 0x%x)", filename, hr);
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool Texture::Initialize(const void* data, uint32_t width, uint32_t height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data;
	initData.SysMemPitch = width * 4;

	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
	if (FAILED(hr))
	{
		XLOG("[Texture] Failed to create texture. HRESULT: 0x%x)", hr);
		return false;
	}

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	if (FAILED(hr))
	{
		XLOG("[Texture] Failed to create shader resource view. HRESULT: 0x%x)", hr);
		return false;
	}

	SafeRelease(texture);
	return true;
}

//----------------------------------------------------------------------------------------------------

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

//----------------------------------------------------------------------------------------------------

void Texture::BindVS(uint32_t index)
{
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(index, 1, &mShaderResourceView);
}

//----------------------------------------------------------------------------------------------------

void Texture::BindPS(uint32_t index)
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(index, 1, &mShaderResourceView);
}