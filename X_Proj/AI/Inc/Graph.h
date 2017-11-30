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
		int x, y;
		Node* neighbors[8];
		size_t neighborCount;

		Node* parent;
		bool bInOpenList, bInClosedList;
		float gCost;
		float hCost;

		Node() : x{ -1 }, y{ -1 }, neighborCount{ 8 }, parent{nullptr}, bInOpenList{false}, bInClosedList{false}, gCost{ 0.0f }, hCost{ 0.0f } {}
	};

private:
	Node mNodes[rows*columns];
	std::list<Node*> mOpenList;
	std::list<Node*> mClosedList;

public:
	Graph();
	~Graph();

	void Initialize(float spacing);
	void Terminate();

	Node* GetNode(size_t x, size_t y);

	std::list<Node*> GetPath() const;
	std::list<Node*> GetClosedList() const;
	bool RunBFS(int startX, int startY, int endX, int endY);
	bool RunDFS(int startX, int startY, int endX, int endY);
	bool RunDijkstra(int startX, int startY, int endX, int endY, float(*TileCostFunction)(const X::Math::Vector2&));
	bool RunAStar(int startX, int startY, int endX, int endY, float(*TileCostFunction)(const X::Math::Vector2&), float (*DirectionCostFunction)(const X::Math::Vector2&, const X::Math::Vector2&));

private:
	void Reset();

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
			node->x = static_cast<int>(x);
			node->y = static_cast<int>(y);

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
} // void Terminate()

template<size_t rows, size_t columns>
typename Graph<rows, columns>::Node* Graph<rows, columns>::GetNode(size_t x, size_t y)
{
	XASSERT(x < columns, "[Graph] Invalid column index.");
	XASSERT(y < rows, "[Graph] Invalid row index.");
	return &mNodes[x + (y * columns)];
} // Node* GetNode(size_t x, size_t y);

template<size_t rows, size_t columns>
std::list<typename Graph<rows, columns>::Node*> Graph<rows, columns>::GetPath() const
{
	std::list<Node*> path;

	if (!mClosedList.empty())
	{
		Node* node = mClosedList.back();
		while (node)
		{
			path.push_front(node);
			node = node->parent;
		}
	}

	return path;
}

template<size_t rows, size_t columns>
std::list<typename Graph<rows, columns>::Node*> Graph<rows, columns>::GetClosedList() const
{
	return mClosedList;
} // std::list<Node*> GetClosedList() const

template<size_t rows, size_t columns>
bool Graph<rows, columns>::RunBFS(int startX, int startY, int endX, int endY)
{
	Reset();

	mOpenList.push_back(GetNode(startX, startY));
	GetNode(startX, startY)->bInOpenList = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Node* node = mOpenList.front();
		mOpenList.pop_front();

		// check if the node is at the destination
		if (node->x == endX && node->y == endY)
		{
			found = true;
		}
		// iterate through the neighbors
		else
		{
			for (int i = 0; i < node->neighborCount; ++i)
			{
				Node* neighbor = node->neighbors[i];
				// skip if the neighbor is in the open or closed list
				if (neighbor->bInClosedList)
				{
					continue;
				}
				if (neighbor->bInOpenList)
				{
					continue;
				}
				// add to open list
				mOpenList.push_back(neighbor);
				neighbor->bInOpenList = true;
				neighbor->parent = node;
			}
		}
		// add the node to the closed list
		mClosedList.push_back(node);
		node->bInClosedList = true;
	}
	return found;
} // bool RunBFS(int startX, int startY, int endX, int endY)

template<size_t rows, size_t columns>
bool Graph<rows, columns>::RunDFS(int startX, int startY, int endX, int endY)
{
	Reset();

	mOpenList.push_back(GetNode(startX, startY));
	GetNode(startX, startY)->bInOpenList = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Node* node = mOpenList.back();
		mOpenList.pop_back();

		// check if the node is at the destination
		if (node->x == endX && node->y == endY)
		{
			found = true;
		}
		// iterate through the neighbors
		else
		{
			for (int i = 0; i < node->neighborCount; ++i)
			{
				Node* neighbor = node->neighbors[i];
				// skip if the neighbor is in the open or closed list
				if (neighbor->bInClosedList)
				{
					continue;
				}
				if (neighbor->bInOpenList)
				{
					continue;
				}
				// add to open list
				mOpenList.push_back(neighbor);
				neighbor->bInOpenList = true;
				neighbor->parent = node;
			}
		}
		// add the node to the closed list
		mClosedList.push_back(node);
		node->bInClosedList = true;
	}
	return found;
} // bool RunDFS(int startX, int startY, int endX, int endY)

template<size_t rows, size_t columns>
bool Graph<rows, columns>::RunDijkstra(int startX, int startY, int endX, int endY, float (*TileCostFunction)(const X::Math::Vector2&))
{
	Reset();

	mOpenList.push_back(GetNode(startX, startY));
	GetNode(startX, startY)->bInOpenList = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Node* node = mOpenList.front();
		mOpenList.pop_front();

		// check if the node is at the destination
		if (node->x == endX && node->y == endY)
		{
			found = true;
		}
		// iterate through the neighbors
		else
		{
			for (int i = 0; i < node->neighborCount; ++i)
			{
				Node* neighbor = node->neighbors[i];
				// skip if the neighbor is in the open or closed list
				if (neighbor->bInClosedList)
				{
					continue;
				}
				if (neighbor->bInOpenList)
				{
					continue;
				}
				// determine cost of moving to this neighbor
				float stepCost = TileCostFunction(X::Math::Vector2(neighbor->x, neighbor->y));
				neighbor->gCost = node->gCost + stepCost;
				// add to open list based on cost
				std::list<Node*>::iterator it;
				for (it = mOpenList.begin(); it != mOpenList.end(); ++it)
				{
					if ((*it)->gCost > neighbor->gCost)
					{
						mOpenList.insert(it, neighbor);
						break;
					}
				}
				if (it == mOpenList.end())
				{
					mOpenList.push_back(neighbor);
				}
				neighbor->bInOpenList = true;
				neighbor->parent = node;
			}
		}
		// add the node to the closed list
		mClosedList.push_back(node);
		node->bInClosedList = true;
	}
	return found;
}

template<size_t rows, size_t columns>
bool Graph<rows, columns>::RunAStar(int startX, int startY, int endX, int endY, float(*TileCostFunction)(const X::Math::Vector2 &), float(*DirectionCostFunction)(const X::Math::Vector2 &, const X::Math::Vector2 &))
{
	Reset();
	mOpenList.push_back(GetNode(startX, startY));
	GetNode(startX, startY)->bInOpenList = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Node* node = mOpenList.front();
		mOpenList.pop_front();

		// check if the node is at the destination
		if (node->x == endX && node->y == endY)
		{
			found = true;
		}
		// iterate through the neighbors
		else
		{
			for (int i = 0; i < node->neighborCount; ++i)
			{
				Node* neighbor = node->neighbors[i];
				// skip if the neighbor is in the open or closed list
				if (neighbor->bInClosedList)
				{
					continue;
				}
				if (neighbor->bInOpenList)
				{
					continue;
				}
				// determine cost of moving to this neighbor
				float gCost = TileCostFunction(X::Math::Vector2(neighbor->x, neighbor->y));
				gCost += node->gCost;
				neighbor->gCost = gCost;

				float hCost = DirectionCostFunction(X::Math::Vector2(node->x, node->y), X::Math::Vector2(neighbor->x, neighbor->y));
				neighbor->hCost = hCost;
				// add to open list based on cost
				std::list<Node*>::iterator it;
				for (it = mOpenList.begin(); it != mOpenList.end(); ++it)
				{
					if (((*it)->gCost + (*it)->hCost)
						> (gCost+hCost))
					{
						mOpenList.insert(it, neighbor);
						break;
					}
				}
				if (it == mOpenList.end())
				{
					mOpenList.push_back(neighbor);
				}
				neighbor->bInOpenList = true;
				neighbor->parent = node;
			}
		}
		// add the node to the closed list
		mClosedList.push_back(node);
		node->bInClosedList = true;
	}
	return found;
}

template<size_t rows, size_t columns>
void Graph<rows, columns>::Reset()
{
	// reset graph nodes
	for (int i = 0; i < rows*columns; ++i)
	{
		Node* node = &mNodes[i];
		node->parent = nullptr;
		node->bInOpenList = false;
		node->bInClosedList = false;
		node->gCost = 0.0f;
	}
	mOpenList.clear();
	mClosedList.clear();
} // void Reset();

} // namespace Ai

#endif // #ifndef INCLUDED_AI_GRAPH_H