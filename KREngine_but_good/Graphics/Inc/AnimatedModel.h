/*
File: Model.h
Author: Jake Roman-Barnes
*/
#pragma once

namespace Graphics
{
	class Bone;
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

	private:
		Bone mRoot;

		struct Part
		{
			Part(SkinnedMesh* mes, MeshBuffer* meshbuffer, size_t matIdx) :mesh(mes), meshBuffer(meshbuffer), materialIndex(matIdx) {}
			SkinnedMesh* mesh;
			MeshBuffer* meshBuffer;
			size_t materialIndex;
			Bone* mBones;
		}; // struct Part

		std::vector<Part> mModelParts;
		std::vector<TextureId> mTextureIds;

	}; // class Model

} // namespace Graphics