/*
File: AnimatedModel.h
Author: Jake Roman-Barnes
*/
#pragma once
#include <vector>
#include "Forward.h"
namespace Graphics
{
	struct Bone;
	class SkinnedMesh;
	class MeshBuffer;
	class Texture;
	class TextureManager;

	class AnimatedModel
	{
	public:
		AnimatedModel();
		~AnimatedModel();

		void Load(const char* filename);
		void Unload();

		void Render();

		Bone* GetRoot() const { return mRoot; }

	private:
		Bone* mRoot;

		struct Part
		{
			Part(SkinnedMesh* mes, MeshBuffer* meshbuffer, size_t matIdx) :mesh(mes), meshBuffer(meshbuffer), materialIndex(matIdx) {}
			SkinnedMesh* mesh;
			MeshBuffer* meshBuffer;
			size_t materialIndex;
		}; // struct Part

		std::vector<Bone*> mBones;
		std::vector<Part> mModelParts;
		std::vector<TextureId> mTextureIds;

	}; // class Model

} // namespace Graphics

