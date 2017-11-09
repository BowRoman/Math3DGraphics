/*
File: AnimatedModel.cpp
Author: Jake Roman-Barnes
*/
#include "Precompiled.h"
#include "AnimatedModel.h"

#include "Bone.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "Texture.h"
#include "TextureManager.h"

#include <cstdio>

namespace Graphics
{

void ScanMatrix(FILE* file, Math::Matrix4& mat)
{
	fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
		&mat._11, &mat._12, &mat._13, &mat._14,
		&mat._21, &mat._22, &mat._23, &mat._24,
		&mat._31, &mat._32, &mat._33, &mat._34,
		&mat._41, &mat._42, &mat._43, &mat._44);
} // void ScanMatrix(FILE* file, Math::Matrix4& mat)


AnimatedModel::AnimatedModel()
{

} // AnimatedModel::AnimatedModel()

AnimatedModel::~AnimatedModel()
{
	ASSERT(mModelParts.empty(), "[AnimatedModel] must free mesh memory");
} // AnimatedModel::~AnimatedModel()

void AnimatedModel::Load(const char* filename)
{
	FILE* file = nullptr;
	errno_t error = fopen_s(&file, filename, "r");
	ASSERT(error == 0, "[AnimatedModel] Error loading model %s", filename);

	uint32_t numMeshes = 0;
	uint32_t numVertices = 0;
	uint32_t numIndices = 0;
	uint32_t numMaterials = 0;
	uint32_t numBones = 0;

	auto result = fscanf_s(file, "MeshCount: %d\n", &numMeshes);
	ASSERT(result == 1, "[Animated Model] Error loading Animated Model (Mesh)");

	for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
	{
		SkinnedMesh* mesh = new SkinnedMesh();
		uint32_t materialIndex = 0;

		result = fscanf_s(file, "VertexCount: %d\n", &numVertices);
		ASSERT(result == 1, "[Animated Model] Error loading Animated Model (Vertices)");
		result = fscanf_s(file, "IndexCount: %d\n", &numIndices);
		ASSERT(result == 1, "[Animated Model] Error loading Animated Model (Indices)");
		result = fscanf_s(file, "MaterialIndex: %d\n", &materialIndex);
		ASSERT(result == 1, "[Animated Model] Error loading Animated Model (Materials)");
		mesh->Allocate(numVertices, numIndices);
		if ((materialIndex + 1) > numMaterials)
		{
			++numMaterials;
		}

		for (uint32_t i = 0; i < numVertices; ++i)
		{
			Graphics::VertexBone& vert = mesh->GetVertex(i);
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
				&vert.position.x, &vert.position.y, &vert.position.z,
				&vert.normal.x, &vert.normal.y, &vert.normal.z,
				&vert.tangent.x, &vert.tangent.y, &vert.tangent.z,
				&vert.uv.x,
				&vert.uv.y);
		}
		for (uint32_t i = 0; i < numIndices; i += 3)
		{
			fscanf_s(file, "%d %d %d\n",
				&mesh->mIndices[i],
				&mesh->mIndices[i + 1],
				&mesh->mIndices[i + 2]);
		}

		for (uint32_t i = 0; i < numVertices; ++i)
		{
			Graphics::VertexBone& vert = mesh->GetVertex(i);
			uint32_t count = 0;
			fscanf_s(file, "%d %d %d %d %d %f %f %f %f\n",
					&count,
					&vert.boneIndex[0],
					&vert.boneIndex[1],
					&vert.boneIndex[2],
					&vert.boneIndex[3],
					&vert.boneWeight[0],
					&vert.boneWeight[1],
					&vert.boneWeight[2],
					&vert.boneWeight[3]);
		}

		Graphics::MeshBuffer* meshBuffer = new MeshBuffer();
		meshBuffer->Initialize(mesh->mVertices, sizeof(Graphics::VertexBone), mesh->mVertexCount, mesh->mIndices, mesh->mIndexCount);

		mModelParts.emplace_back(Part(mesh, meshBuffer, materialIndex));
	}

	for (uint32_t i = 0; i < numMaterials; ++i)
	{
		char materialFilepath[1024];
		fscanf_s(file, "MaterialMap: %s\n", materialFilepath, 1024);
		if (materialFilepath == "none")
		{
			strcpy(materialFilepath, "error.jpg");
		}
		TextureId hash = TextureManager::Get()->Load(materialFilepath);
		mTextureIds.emplace_back(hash);
	}

	result = fscanf_s(file, "BoneCount: %d\n", &numBones);
	ASSERT(result == 1, "[Animated Model] Error loading Animated Model");
	mBones.reserve(numBones);
	for (uint32_t BoneIndex = 0; BoneIndex < numBones; ++BoneIndex)
	{
		Bone* bone = new Bone();
		// read name and index
		char boneName[1024];
		fscanf_s(file, "Name: %s\n", boneName, 1024);
		bone->name = boneName;

		fscanf_s(file, "Index: %d\n", &bone->index);

		// read parent index
		fscanf_s(file, "ParentIndex: %d\n", &bone->parentIndex);

		// read each child
		uint32_t numChildren = 0;
		fscanf_s(file, "ChildCount: %d\n", &numChildren);
		for (uint32_t i = 0; i < numChildren; ++i)
		{
			uint32_t childIdx = 0;
			fscanf_s(file, "%d ", &childIdx);
			bone->childrenIndex.push_back(childIdx);
		}
		fscanf_s(file, "\n");

		// read matrices
		Math::Matrix4 transformMat;

		ScanMatrix(file, transformMat);
		bone->transform = transformMat;

		ScanMatrix(file, transformMat);
		bone->offsetTransform = transformMat;

		mBones.push_back(bone);
	}
	
	for (auto& bone : mBones)
	{
		// if the bone has no parent, set as root. Otherwise set the parent pointer
		if (bone->parentIndex == -1)
		{
			bone->parent = nullptr;
			mRoot = mBones[bone->index];
		}
		else
		{
			bone->parent = mBones[bone->parentIndex];
		}
		// set children pointers
		for (auto childIndex : bone->childrenIndex)
		{
			bone->children.push_back(mBones[childIndex]);
		}
	}

	fclose(file);
} // void AnimatedModel::Load(const char* filename)

void AnimatedModel::Unload()
{
	for (auto& it : mModelParts)
	{
		it.mesh->Terminate();
		SafeDelete(it.mesh);

		it.meshBuffer->Terminate();
		SafeDelete(it.meshBuffer);
	}
	mModelParts.clear();
	mTextureIds.clear();
	mBones.clear();
	SafeDeleteVector(mBones);
} // void AnimatedModel::Unload()

void AnimatedModel::Render()
{
	for (auto& part : mModelParts)
	{
		TextureManager::Get()->BindVS(mTextureIds[part.materialIndex], 0);
		TextureManager::Get()->BindPS(mTextureIds[part.materialIndex], 0);
		part.meshBuffer->Render();
	}

} // void AnimatedModel::Render()

} // namespace Graphics