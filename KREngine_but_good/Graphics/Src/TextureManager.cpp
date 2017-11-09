#include "Precompiled.h"

#include "TextureManager.h"

#include "Texture.h"

using namespace Graphics;

namespace
{
	TextureManager* sTextureManager = nullptr;
} // namespace

void TextureManager::StaticInitialize(const char* root)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] TextureManager already initialized!");
	sTextureManager = new TextureManager();
	if (root)
	{
		sTextureManager->SetRootPath(root);
	}
} // void StaticInitialize(const char* root)

void TextureManager::StaticTerminate()
{
	if (sTextureManager)
	{
		SafeDelete(sTextureManager);
	}
} // void StaticTerminate()

TextureManager* TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "[Graphics::TextureManager] No manager registered.");
	return sTextureManager;
} // TextureManager* Get()

TextureManager::TextureManager()
{
} // TextureManager()

TextureManager::~TextureManager()
{
	for (auto& item : mInventory)
	{
		item.second->Terminate();
		SafeDelete(item.second);
	}
	mInventory.clear();
} // ~TextureManager()

void TextureManager::SetRootPath(const char* root)
{
	mRoot = root;
} // void SetRootPath(const char* root)

// Returns a hashed index for the corresponding texture
int TextureManager::Load(const char* filename)
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
} // int Load(const char* filename)

void Graphics::TextureManager::BindVS(TextureId id, uint32_t slot)
{
	mInventory[id]->BindVS(slot);
} // void BindVS(TextureId id, uint32_t slot)

void Graphics::TextureManager::BindPS(TextureId id, uint32_t slot)
{
	mInventory[id]->BindPS(slot);
} // void BindPS(TextureId id, uint32_t slot)