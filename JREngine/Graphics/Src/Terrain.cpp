#include "Precompiled.h"
#include "Terrain.h"

#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "Texture.h"
#include "Sampler.h"
#include "VertexTypes.h"

#include <fstream>

using namespace Graphics;

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::Initialize(const char *rawFileName, uint32_t columns, uint32_t rows, float scale, Math::Vector3 origin, uint32_t distBtwnVert)
{
	mMesh.Allocate(columns*rows, (columns * 2) * (rows - 1) + (rows - 2));

	// move the terrain starting point from the center to the top left
	const Math::Vector3 startPoint = origin - Math::Vector3(columns*0.5f*distBtwnVert, 0.f, -(rows*0.5f*distBtwnVert));
	Math::Vector3 currVert = startPoint;

	unsigned char *file = new unsigned char[columns * rows * 4];

	// read uv map and interpret into floats to use for height
	std::ifstream heightStream;
	heightStream.open(rawFileName, std::ios::binary);
	heightStream.read((char*)file, columns * rows * 4);
	heightStream.close();
	//float *verticesHeight = reinterpret_cast<float*>(file);

	// set indices in CPU mesh
	int index = 0;
	for (int z = 0; z < rows - 1; z++)
	{
		// Even rows move left to right, odd rows move right to left.
		if (z % 2 == 0)
		{
			// Even row
			int x;
			for (x = 0; x < columns; x++)
			{
				mMesh.mIndices[index++] = static_cast<uint32_t>(x + (z * columns));
				mMesh.mIndices[index++] = static_cast<uint32_t>(x + (z * columns) + columns);
			}
			// Insert degenerate vertex if this isn't the last row
			if (z != rows - 2)
			{
				mMesh.mIndices[index++] = static_cast<uint32_t>(--x + (z * columns));
			}
		}
		else
		{
			// Odd row
			int x;
			for (x = columns - 1; x >= 0; x--)
			{
				mMesh.mIndices[index++] = static_cast<uint32_t>(x + (z * columns));
				mMesh.mIndices[index++] = static_cast<uint32_t>(x + (z * columns) + columns);
			}
			// Insert degenerate vertex if this isn't the last row
			if (z != rows - 2)
			{
				mMesh.mIndices[index++] = static_cast<uint32_t>(++x + (z * columns));
			}
		}
	}

	// create vertex buffer array
	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < columns; ++j)
		{
			currVert = startPoint;
			currVert.x += static_cast<float>(j*distBtwnVert);
			currVert.y = (float)file[i + j] * 10.f; // TODO: replace with a parameter factor
			currVert.z -= static_cast<float>(i*distBtwnVert);
			mMesh.mVertices[i + j].position = currVert;
			mMesh.mVertices[i + j].uv = Math::Vector2((float)j, (float)i);
		}
	}

	SafeDeleteArray(file);


	// set vertices and indices in GPU mesh
	mMeshBuffer.Initialize(mMesh.mVertices, sizeof(Graphics::Vertex), mMesh.mNumVertices, mMesh.mIndices, mMesh.mNumIndices);
}

void Terrain::Terminate()
{
	mMeshBuffer.Terminate();
}

void Terrain::Render()
{
	mMeshBuffer.Render();
}
