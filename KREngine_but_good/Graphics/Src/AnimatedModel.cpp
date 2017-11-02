/*
File: Model.cpp
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

namespace Graphics {

	AnimatedModel::AnimatedModel()
	{

	}
	AnimatedModel::~AnimatedModel()
	{
		ASSERT(mModelParts.empty(), "[AnimatedModel] must free mesh memory");
	}

	void AnimatedModel::Load(const char* filename)
	{
		FILE* file = nullptr;
		errno_t error = fopen_s(&file, filename, "r");
		ASSERT(error == 0, "[AnimatedModel] Error loading model %s", filename);

		uint32_t numMeshes = 0;
		uint32_t numVertices = 0;
		uint32_t numIndices = 0;
		uint32_t numMaterials = 0;

		fscanf_s(file, "MeshCount: %d\n", &numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			SkinnedMesh* mesh = new SkinnedMesh();
			uint32_t materialIndex = 0;

			fscanf_s(file, "VertexCount: %d\n", &numVertices);
			fscanf_s(file, "IndexCount: %d\n", &numIndices);
			fscanf_s(file, "MaterialIndex: %d\n", &materialIndex);
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

			// TODO: Read bone weights

			Graphics::MeshBuffer* meshBuffer = new MeshBuffer();
			meshBuffer->Initialize(mesh->mVertices, sizeof(Graphics::Vertex), mesh->mVertexCount, mesh->mIndices, mesh->mIndexCount);

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

		// TODO: Read bones
		// if parent = -1 mRoot = i

		fclose(file);
	}
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
	}

	void AnimatedModel::Render()
	{
		for (auto& part : mModelParts)
		{
			TextureManager::Get()->BindVS(mTextureIds[part.materialIndex], 0);
			TextureManager::Get()->BindPS(mTextureIds[part.materialIndex], 0);
			part.meshBuffer->Render();
		}
	}

}