#include <Graphics\Inc\Graphics.h>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>

using namespace Graphics;


#pragma comment(lib, "assimp.lib")
struct Params
{
	Params()
		: inputFilename(nullptr)
		, outputFilename(nullptr)
	{}

	const char* inputFilename;
	const char* outputFilename;
};

struct BoneWeights
{
	uint32_t index[4] = {};
	float weight[4] = {};
	uint32_t count = 0;
};

typedef std::map<std::string, uint32_t> BoneIndexMap;
typedef std::vector<Bone*> Bones;

Math::Matrix4 Convert(const aiMatrix4x4& aiMatrix)
{
	Math::Matrix4 mat = *(Math::Matrix4*)&aiMatrix;
	return Math::Transpose(mat);
}

// search to see if the bone has already been added, if it hasn't then add it to the bones array and add a lookup entry to the map
uint32_t GetBoneIndex(aiBone* bone, Bones& bones, BoneIndexMap& boneIndexMap)
{
	// if the bone is already in the map, return its index
	auto it = boneIndexMap.find(bone->mName.C_Str());
	if (it != boneIndexMap.end())
	{
		return it->second;
	}

	// new bone is needed
	uint32_t boneIndex = bones.size();
	Bone* newBone = new Bone();
	ASSERT(bone->mName.length > 0, "Warning: Bone %d has no name.", boneIndex);

	newBone->name = bone->mName.C_Str();
	newBone->index = boneIndex;
	newBone->offsetTransform = Convert(bone->mOffsetMatrix);

	bones.push_back(newBone);
	boneIndexMap.insert(std::make_pair(bone->mName.C_Str(), boneIndex));

	return boneIndex;
}

Bone* BuildSkeleton(aiNode& ainode, Bone* parent, Bones& bones, BoneIndexMap& boneIndexMap)
{
	Bone* bone = nullptr;

	auto it = boneIndexMap.find(ainode.mName.C_Str());
	if (it == boneIndexMap.end())
	{
		const uint32_t boneIndex = bones.size();

		bone = new Bone();
		bone->index = boneIndex;
		bone->offsetTransform = Math::Matrix4::Identity();

		if (ainode.mName.length > 0)
		{
			bone->name = ainode.mName.C_Str();
		}
		else
		{
			char buffer[128];
			sprintf_s(buffer, 128, "unnamedBone_%u", boneIndex);
			printf("Warning. Bone %u is unnamed, renamed \"%s\"\n", boneIndex, buffer);
			bone->name = buffer;
		}

		bones.push_back(bone);
		boneIndexMap.insert(std::make_pair(bone->name, bone->index));
	}
	else
	{
		bone = bones[it->second];
	}

	bone->transform = Convert(ainode.mTransformation);
	bone->parent = parent;
	bone->parentIndex = parent ? parent->index : -1;

	for (uint32_t i = 0; i < ainode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*(ainode.mChildren[i]), bone, bones, boneIndexMap);
		bone->children.push_back(child);
		bone->childrenIndex.push_back(child->index);
	}

	return bone;
}

const char* StripPath(const char* filePath)
{
	// if the path uses forward slash, grab the front
	const char* fileName = strrchr(filePath, '\\');
	if (fileName != nullptr)
	{
		return ++fileName;
	}

	// check if it uses backslash instead
	fileName = strrchr(filePath, '/');
	if (fileName != nullptr)
	{
		return ++fileName;
	}

	return filePath;
}

void PrintHelp()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"\n"
		"\tModelImporter.exe <InputFile> <OutputFile>\n"
		"\n"
	);
}

void PrintMatrix(FILE* file, const Math::Matrix4& mat)
{
	fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			mat._11, mat._12, mat._13, mat._14,
			mat._21, mat._22, mat._23, mat._24, 
			mat._31, mat._32, mat._33, mat._34, 
			mat._41, mat._42, mat._43, mat._44
	);
}


bool ParseArg(int argc, char* argv[], Params& params)
{
	// We need at least 3 arguments
	if (argc < 3)
	{
		return false;
	}

	params.inputFilename = argv[argc - 2];
	params.outputFilename = argv[argc - 1];

	return true;
}


bool ImportModel(const Params& params)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const uint32_t flags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_MakeLeftHanded |
		aiProcess_GenNormals |
		aiProcess_FlipUVs;
	const aiScene* scene = importer.ReadFile(params.inputFilename, flags);

	// If the import failed, report it
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return false;
	}

	// Open a new file for saving
	FILE* file = nullptr;
	errno_t error = fopen_s(&file, params.outputFilename, "w");
	if (error != 0)
	{
		printf("Error: Failed to write to output file.\n");
		return false;
	}

	BoneIndexMap boneIndexMap;
	Bones bones;

	// Read mesh data
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");
		fprintf(file, "MeshCount: %d\n", scene->mNumMeshes);

		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* aiMesh = scene->mMeshes[meshIndex];

			fprintf(file, "VertexCount: %d\n", aiMesh->mNumVertices);
			fprintf(file, "IndexCount: %d\n", aiMesh->mNumFaces * 3);
			fprintf(file, "MaterialIndex: %d\n", aiMesh->mMaterialIndex);
			
			printf("Reading vertices...\n");
			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->mTangents;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;

			for (uint32_t i = 0; i < aiMesh->mNumVertices; ++i)
			{
				fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
					positions[i].x, positions[i].y, positions[i].z,
					normals[i].x, normals[i].y, normals[i].z,
					tangents[i].x, tangents[i].y, tangents[i].z,
					texCoords ? texCoords[i].x : 0.0f,
					texCoords ? texCoords[i].y : 0.0f);
			}

			// indices
			printf("Reading indices...\n");

			for (uint32_t i = 0; i < aiMesh->mNumFaces; ++i)
			{
				fprintf(file, "%d %d %d\n",
					aiMesh->mFaces[i].mIndices[0],
					aiMesh->mFaces[i].mIndices[1],
					aiMesh->mFaces[i].mIndices[2]);
			}

			if (aiMesh->HasBones())
			{
				// bones
				printf("Reading bone weights...\n");

				std::vector<BoneWeights> boneWeights;
				boneWeights.resize(aiMesh->mNumVertices);

				for (uint32_t i = 0; i < aiMesh->mNumBones; ++i)
				{
					aiBone* aiBone = aiMesh->mBones[i];
					uint32_t boneIndex = GetBoneIndex(aiBone, bones, boneIndexMap);

					for (uint32_t j = 0; j < aiBone->mNumWeights; ++j)
					{
						// Grab the current weight
						const aiVertexWeight& aiVertexWeight = aiBone->mWeights[j];
						// Get which vertex this weight is effecting
						const uint32_t vertexIndex = aiVertexWeight.mVertexId;

						// If your mesh has more than 4 weights per bone you don't deserve to have your model loaded
						if (boneWeights[vertexIndex].count < 4)
						{
							int weightIndex = boneWeights[vertexIndex].count++;

							// Set necessary bone weight info for the current vertex
							boneWeights[vertexIndex].index[weightIndex] = boneIndex;
							boneWeights[vertexIndex].weight[weightIndex] = aiVertexWeight.mWeight;
						}
					}
				}

				for (auto boneWeight : boneWeights)
				{
					fprintf(file, "%d %d %d %d %d %f %f %f %f\n",
						boneWeight.count,
						boneWeight.index[0],
						boneWeight.index[1],
						boneWeight.index[2],
						boneWeight.index[3],
						boneWeight.weight[0],
						boneWeight.weight[1],
						boneWeight.weight[2],
						boneWeight.weight[3]);
				}
			}
		}
	}

	if (scene->HasMaterials())
	{
		printf("Reading materials...\n");

		for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
		{
			const aiMaterial* material = scene->mMaterials[i];
			uint32_t diffuseMapCount = material->GetTextureCount(aiTextureType_DIFFUSE);
			uint32_t specularMapCount = material->GetTextureCount(aiTextureType_SPECULAR);
			uint32_t normalMapCount = material->GetTextureCount(aiTextureType_NORMALS);
			if (diffuseMapCount > 0)
			{
				aiString diffuseFilepath;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseFilepath) == AI_SUCCESS)
				{
					std::string str = StripPath(diffuseFilepath.C_Str());
					//str.replace(str.end() - 4, str.end(), ".dds");
					fprintf(file, "MaterialMap: %s\n", str.c_str());
				}
				else
				{
					fprintf(file, "MaterialMap: none\n");
				}
			}
			if (specularMapCount > 0)
			{
				aiString specularFilepath;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &specularFilepath) == AI_SUCCESS)
				{
					std::string str = StripPath(specularFilepath.C_Str());
					//str.replace(str.end() - 4, str.end(), ".dds");
					fprintf(file, "MaterialMap: %s\n", str.c_str());
				}
				else
				{
					fprintf(file, "MaterialMap: none\n");
				}
			}
			if (normalMapCount > 0)
			{
				aiString normalFilepath;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &normalFilepath) == AI_SUCCESS)
				{
					std::string str = StripPath(normalFilepath.C_Str());
					//str.replace(str.end() - 4, str.end(), ".dds");
					fprintf(file, "MaterialMap: %s\n", str.c_str());
				}
				else
				{
					fprintf(file, "MaterialMap: none\n");
				}
			}
		}
	}

	if (scene->HasAnimations())
	{
		printf("Reading skeleton...\n");
		Bone* root = BuildSkeleton(*scene->mRootNode, nullptr, bones, boneIndexMap);

		fprintf(file, "BoneCount: %d\n", bones.size());
		for (auto bone : bones)
		{
			fprintf(file, "Name: %s\n", bone->name.c_str());
			fprintf(file, "Index: %d\n", bone->index);
			fprintf(file, "ParentIndex: %d\n", bone->parentIndex);
			fprintf(file, "ChildCount: %d\n", bone->children.size());
			if (!bone->children.empty())
			{
				for (auto childIndex : bone->childrenIndex)
				{
					fprintf(file, "%d ", childIndex);
				}
				fprintf(file, "\n");
			}
			PrintMatrix(file, bone->transform);
			PrintMatrix(file, bone->offsetTransform);
		}
	}

	// All done!
	printf("Done!\n");
	fclose(file);
	return true;
}

int main(int argc, char* argv[])
{
	Params params;
	if (!ParseArg(argc, argv, params))
	{
		PrintHelp();
		return -1;
	}

	if (!ImportModel(params))
	{
		printf("Failed to import model %s.\n", params.inputFilename);
		return -1;
	}

	return 0;
}