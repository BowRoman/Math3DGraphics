#include "Precompiled.h"
#include "HeightMap.h"
#include <fstream>


Graphics::HeightMap::HeightMap()
	:mHeightVertices(nullptr),
	mColumns(0),
	mRows(0)
{
}

Graphics::HeightMap::~HeightMap()
{
}

void Graphics::HeightMap::Initialize(const char* fileName, unsigned int cols, unsigned int rows)
{
	ASSERT(cols > 0, "[HeightMap] Invalid number of columns");
	ASSERT(rows > 0, "[HeightMap] Invalid number of rows");

	mColumns = cols;
	mRows = rows;

	std::ifstream file(fileName, std::ios::binary);
	size_t bytesSize = cols * rows * 4;
	unsigned char* buffer = new unsigned char[bytesSize];
	file.read((char*)buffer, bytesSize);
	file.close();
	mHeightVertices = reinterpret_cast<float*>(buffer);

	float tempMin = 0;
	float tempMax = 0;
	for (uint32_t i = 0; i < mColumns * mRows; ++i)
	{
		float tempVal = mHeightVertices[i];
		if (tempVal < tempMin)
		{
			tempVal = tempMin;
		}
		if (tempVal > tempMax)
		{
			tempMax = tempVal;
		}
	}
}

// http://www.chadvernon.com/blog/resources/directx9/terrain-generation-with-a-heightmap/
void Graphics::HeightMap::GenerateIndices(uint32_t** indices, int &indexCount)
{
	indexCount = (mColumns * 2) * (mRows - 1) + (mRows - 2);

	SafeDeleteArray(*indices);

	*indices = new uint32_t[indexCount];
	int index = 0;

	int length;
	for (int width = 0; width < mRows - 1; ++width)
	{
		length = 0;
		if (width % 2 == 0) // even rows left to right
		{
			for (length = 0; length < mColumns; ++length)
			{
				(*indices)[index++] = length + (width * mColumns);
				(*indices)[index++] = length + (width * mColumns) + mColumns;
			}
			if (width != mRows - 2)
			{
				(*indices)[index++] = --length + (width * mColumns);
			}
		}
		else // odd rows right to left
		{
			for (length = mColumns - 1; length >= 0; --length)
			{
				(*indices)[index++] = length + (width * mColumns);
				(*indices)[index++] = length + (width * mColumns) + mColumns;
			}
			if (width != mRows - 2)
			{
				(*indices)[index++] = ++length + (width * mColumns);
			}
		}
	}
}

void Graphics::HeightMap::Terminate()
{
	SafeDelete(mHeightVertices);
}

float Graphics::HeightMap::GetHeight(uint32_t row, uint32_t col) const
{
	ASSERT(row < mColumns && col < mRows, "[HeightMap] Out of range");
	return mHeightVertices[row + (col * mColumns)];
}

uint32_t Graphics::HeightMap::GetColumns() const
{
	return mColumns;
}

uint32_t Graphics::HeightMap::GetRows() const
{
	return mRows;
}
