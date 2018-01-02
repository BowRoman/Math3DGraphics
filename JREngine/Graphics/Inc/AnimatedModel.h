/*
File: AnimatedModel.h
Author: Jake Roman-Barnes
*/
#pragma once
#include <vector>
#include "AnimationClip.h"
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
private:
	void PropegateBoneMatrices(uint32_t boneIndex);

public:
	AnimatedModel();
	~AnimatedModel();

	void Load(const char* filename);
	void Unload();

	void Play();
	void Pause();
	void Reset();

	void Update(float deltaTime);

	void Render();

	Bone* GetRoot() const { return mRoot; }
	Math::Matrix4 GetBoneTransform(uint32_t index) const { return mBoneMatrices[index]; }

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
	std::vector<Math::Matrix4> mBoneMatrices;
	std::vector<Part> mModelParts;
	std::vector<TextureId> mTextureIds;
	std::vector<AnimationClip> mAnimationClips;
	size_t mClipIndex;

}; // class Model

} // namespace Graphics

