#pragma once

namespace Graphics
{

class TextureManager
{
public:
	static void StaticInitialize(const char* root = nullptr);
	static void StaticTerminate();
	static TextureManager* Get();

public:
	TextureManager();
	~TextureManager();

	void SetRootPath(const char* root);

	int Load(const char* filename);

	void BindVS(TextureId id, uint32_t slot = 0);
	void BindPS(TextureId id, uint32_t slot = 0);

private:
	std::string mRoot;
	std::unordered_map<TextureId, Graphics::Texture*> mInventory;
};

}