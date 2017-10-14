/*
File: Model.h
Author: Jake Roman-Barnes
*/
#pragma once

namespace Graphics {

class Mesh;
class MeshBuffer;
class Texture;
class TextureManager;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* filename);
	void Unload();

	void Render();

private:
	struct Part
	{
		Part(Mesh* mes, MeshBuffer* meshbuffer, size_t matIdx) :mesh(mes), meshBuffer(meshbuffer), materialIndex(matIdx) {}
		Mesh* mesh;
		MeshBuffer* meshBuffer;
		size_t materialIndex;
	};
	std::vector<Part> mModelParts;
	std::vector<TextureId> mTextureIds;
};

}