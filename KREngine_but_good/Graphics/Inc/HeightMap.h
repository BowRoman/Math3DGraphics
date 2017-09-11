#pragma once


namespace Graphics
{
	class HeightMap
	{
		float* mHeightVertices;
		uint32_t mColumns;
		uint32_t mRows;
	public:
		HeightMap();
		~HeightMap();
		void Initialize(const char* fileName, unsigned int cols, unsigned int rows);
		void Terminate();

		void GenerateIndices(uint32_t** indices, int &indexCount);
		float GetHeight(uint32_t row, uint32_t col) const;
		uint32_t GetColumns() const;
		uint32_t GetRows() const;

	private:
	};

}