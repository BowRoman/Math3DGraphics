#include "Precompiled.h"

#include "TextureManager.h"

#include "Texture.h"

using namespace Graphics;

namespace
{
	TextureManager* sTextureManager = nullptr;
}

void TextureManager::StaticInitialize(const char * root)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] TextureManager already initialized!");
	sTextureManager = new TextureManager();
	if (root)
	{
		sTextureManager->SetRootPath(root);
	}
}

void TextureManager::StaticTerminate()
{
	if (sTextureManager)
	{
		SafeDelete(sTextureManager);
	}
}

TextureManager* TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "[Graphics::TextureManager] No manager registered.");
	return sTextureManager;
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (auto& item : mInventory)
	{
		item.second->Terminate();
		SafeDelete(item.second);
	}
	mInventory.clear();
}

void TextureManager::SetRootPath(const char * root)
{
	mRoot = root;
}

int TextureManager::Load(const char * filename)
{
	std::string fullname = mRoot + filename;
	std::hash<std::string> hasher;
	TextureId hash = hasher(fullname);

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		Texture* texture = new Texture();
		texture->Initialize(fullname.c_str());
		result.first->second = texture;
	}

	return hash;
}

void Graphics::TextureManager::BindVS(TextureId id, uint32_t slot)
{
	mInventory[id]->BindVS(slot);
}

void Graphics::TextureManager::BindPS(TextureId id, uint32_t slot)
{
	mInventory[id]->BindPS(slot);
}