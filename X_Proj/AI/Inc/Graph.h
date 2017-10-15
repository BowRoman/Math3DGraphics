#ifndef INCLUDED_AI_GRAPH_H
#define INCLUDED_AI_GRAPH_H

#include <XEngine.h>

namespace Ai
{

template<size_t rows, size_t columns>
class Graph
{
public:
	struct Node
	{
		X::Math::Vector2 position;
		Node* neighbors[8];
		size_t neighborCount = 0;
		//float gCost;
		//float hCost;
		//Node* parent;
	};
private:
	Node mNodes[rows*columns];
public:
	Graph();
	~Graph();

	void Initialize(float spacing);
	void Terminate();

	Node* GetNode(size_t x, size_t y);

}; // class Graph

template<size_t rows, size_t columns>
Graph<rows, columns>::Graph()
{

} // Graph::Graph()

template<size_t rows, size_t columns>
Graph<rows, columns>::~Graph()
{

} // Graph::~Graph()

template<size_t rows, size_t columns>
void Graph<rows, columns>::Initialize(float spacing)
{
	for (size_t y = 0; y < rows; ++y)
	{
		for (size_t x = 0; x < columns; ++x)
		{
			const size_t index = x + (y*columns);
			Node* node = &mNodes[index];
			node->position = X::Math::Vector2(x*spacing, y*spacing);

			size_t count = 0;
			// left column
			if (x > 0)
			{
				// left
				node->neighbors[count++] = &mNodes[(x - 1) + (y * columns)];
				// left top
				if (y > 0)
				{
					node->neighbors[count++] = &mNodes[(x - 1) + ((y - 1) * columns)];
				}
				// left bottom
				if (y + 1 < rows)
				{
					node->neighbors[count++] = &mNodes[(x - 1) + ((y + 1) * columns)];
				}
			}
			// top
			if (y > 0)
			{
				node->neighbors[count++] = &mNodes[x + ((y - 1) * columns)];
			}
			// bottom
			if (y + 1 < rows)
			{
				node->neighbors[count++] = &mNodes[x + ((y + 1) * columns)];
			}
			// right column
			if (x + 1 < columns)
			{
				// right
				node->neighbors[count++] = &mNodes[(x + 1) + (y * columns)];
				// right top
				if (y > 0)
				{
					node->neighbors[count++] = &mNodes[(x + 1) + ((y - 1) * columns)];
				}
				// right bottom
				if (y + 1 < rows)
				{
					node->neighbors[count++] = &mNodes[(x + 1) + ((y + 1) * columns)];
				}
			}
			node->neighborCount = count;
		}
	}
} // void Initialize(float spacing);

template<size_t rows, size_t columns>
void Graph<rows, columns>::Terminate()
{
}

template<size_t rows, size_t columns>
typename Graph<rows, columns>::Node* Graph<rows, columns>::GetNode(size_t x, size_t y)
{
	XASSERT(x < columns, "[Graph] Invalid column index.");
	XASSERT(y < rows, "[Graph] Invalid row index.");
	return &mNodes[x + (y * columns)];
} // Node* GetNode(size_t x, size_t y);

} // namespace Ai

#endif // #ifndef INCLUDED_AI_GRAPH_H