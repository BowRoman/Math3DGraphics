#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <cstdlib>
#include <cstdio>

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

	// Read mesh data
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");
		fprintf(file, "MeshCount: %d\n", scene->mNumMeshes);

		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* aiMesh = scene->mMeshes[meshIndex];

			printf("Reading vertices...\n");
			fprintf(file, "VertexCount: %d\n", aiMesh->mNumVertices);
			fprintf(file, "IndexCount: %d\n", aiMesh->mNumFaces * 3);
			fprintf(file, "MaterialIndex: %d\n", aiMesh->mMaterialIndex);

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

			printf("Reading indices...\n");

			for (uint32_t i = 0; i < aiMesh->mNumFaces; ++i)
			{
				fprintf(file, "%d %d %d\n",
					aiMesh->mFaces[i].mIndices[0],
					aiMesh->mFaces[i].mIndices[1],
					aiMesh->mFaces[i].mIndices[2]);
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