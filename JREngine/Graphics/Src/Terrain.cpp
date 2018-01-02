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
	:mHeightVertices(nullptr),
	mColumns(0),
	mRows(0)
{
}

Terrain::~Terrain()
{
}

void Terrain::Initialize(const char* rawFileName, uint32_t cols, uint32_t rows, float scale, float maxHeight, Math::Vector3 origin)
{
	ASSERT(cols > 0, "[HeightMap] Invalid number of columns");
	ASSERT(rows > 0, "[HeightMap] Invalid number of rows");

	mColumns = cols;
	mRows = rows;

	std::ifstream file(rawFileName, std::ios::binary);
	size_t bytesSize = cols * rows * 4;

	unsigned char* buffer = new unsigned char[bytesSize];
	file.read((char*)buffer, bytesSize);
	file.close();
	mHeightVertices = reinterpret_cast<float*>(buffer);

	float tempMin = 0;
	for (uint32_t i = 0; i < mColumns * mRows; ++i)
	{
		float tempVal = mHeightVertices[i];
		if (tempVal < tempMin)
		{
			tempVal = tempMin;
		}
		if (tempVal > maxHeight)
		{
			tempVal = maxHeight;
		}
		mHeightVertices[i] = tempVal;
	}

	uint32_t intcount = (mColumns * 2) * (mRows - 1) + (mRows - 2);
	mMesh.Allocate(rows*cols, intcount);

	float startingX = origin.x - ((rows*scale)*0.5f);
	float startingZ = origin.z - ((cols*scale)*0.5f);
	for (uint32_t i = 0; i < cols; ++i)
	{
		for (uint32_t j = 0; j < rows; ++j)
		{
			mMesh.mVertices[(i * cols) + j].position.y = origin.y + (GetHeight(i, j) * scale);
			mMesh.mVertices[(i * cols) + j].position.x = startingX + (j * scale);
			mMesh.mVertices[(i * cols) + j].position.z = startingZ + (i * scale);
			mMesh.mVertices[(i * cols) + j].normal = Math::Vector3::YAxis();
			mMesh.mVertices[(i * cols) + j].tangent = Math::Vector3::XAxis();
			mMesh.mVertices[(i * cols) + j].color = Math::Vector4::White();
			mMesh.mVertices[(i * cols) + j].uv.x = (float)j / (float)cols;
			mMesh.mVertices[(i * cols) + j].uv.y = (float)i / (float)cols;
		}
	}

	GenerateIndices();

	mMeshBuffer.Initialize(mMesh.GetVertices(), sizeof(Graphics::Vertex), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());
	mMeshBuffer.SetTopology(Graphics::MeshBuffer::Topology::TriangleStrip);
}

void Terrain::Terminate()
{
	SafeDelete(mHeightVertices);
	mMesh.Destroy();
	mMeshBuffer.Terminate();
}

float Terrain::GetHeight(uint32_t row, uint32_t col) const
{
	ASSERT(row < mColumns && col < mRows, "[HeightMap] Out of range");
	return mHeightVertices[row + (col * mColumns)];
}

uint32_t Graphics::Terrain::GetColumns() const
{
	return mColumns;
}

uint32_t Graphics::Terrain::GetRows() const
{
	return mRows;
}

Mesh& Graphics::Terrain::GetMesh()
{
	return mMesh;
}

void Graphics::Terrain::Render()
{
	mMeshBuffer.Render();
}

// http://www.chadvernon.com/blog/resources/directx9/terrain-generation-with-a-heightmap/
void Terrain::GenerateIndices()
{
	uint32_t numIndices = mMesh.GetIndexCount();
	uint32_t* indices = new uint32_t[numIndices];
	int index = 0;
	int length = 0;

	for (int width = 0; width < static_cast<int>(mRows) - 1; ++width)
	{
		length = 0;
		if (width % 2 == 0) // even rows left to right
		{
			for (length = 0; length < static_cast<int>(mColumns); ++length)
			{
				(indices)[index++] = length + (width * mColumns);
				(indices)[index++] = length + (width * mColumns) + mColumns;
			}
			if (width != mRows - 2)
			{
				(indices)[index++] = --length + (width * mColumns);
			}
		}
		else // odd rows right to left
		{
			for (length = mColumns - 1; length >= 0; --length)
			{
				(indices)[index++] = length + (width * mColumns);
				(indices)[index++] = length + (width * mColumns) + mColumns;
			}
			if (width != mRows - 2)
			{
				(indices)[index++] = ++length + (width * mColumns);
			}
		}
	}
	for (int i = 0; i < static_cast<int>(numIndices); ++i)
	{
		mMesh.mIndices[i] = indices[i];
	}
	SafeDeleteArray(indices);
}