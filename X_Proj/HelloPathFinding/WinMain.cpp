#include <Ai.h>
#include <XEngine.h>

const size_t numRows = 24;
const size_t numColumns = 32;
typedef Ai::Graph<numRows, numColumns> NavGraph;
typedef Ai::Graph<numRows, numColumns>::Node NavNode;

enum class TileType
{
	None		= 0,
	Grass		= 1,
	Flower		= 2,
	TreeTL		= 3,
	TreeTR		= 4,
	TreeBL		= 5,
	TreeBR		= 6,
	PathStart	= 7,
	PathEnd		= 8
};

const char* const textureNames[]
{
	"white.jpg",
	"grass.png",
	"flower.png",
	"tree0.png",
	"tree1.png",
	"tree2.png",
	"tree3.png"
};
X::TextureId textureIds[std::size(textureNames)];

X::TextureId startPointId;
X::TextureId endPointId;
int startX = -1, startY = -1, endX = -1, endY = -1;
std::list<Ai::Graph<24, 32>::Node*> path;
enum SearchType
{
	DFS = 0,
	BFS = 1
};
SearchType searchMethod = BFS;

const float tileSize = 32.0f;
NavGraph graph;
const size_t tileMap[]
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1,
	1, 3, 4, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 6, 1, 1,
	1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 2, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1,
	1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 4, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 3, 4, 3, 4, 1, 1, 1, 1, 1, 3, 4, 1,
	1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 6, 5, 6, 1, 1, 1, 1, 1, 5, 6, 1,
	1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 5, 6, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 4, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1,
	1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 5, 6, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 3, 4, 1, 1, 1, 1, 1, 2, 1, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void DrawMapTiles()
{
	// draw map
	for (int y = 0; y < numRows; ++y)
	{
		for (int x = 0; x < numColumns; ++x)
		{
			X::Math::Vector2 position(tileSize*x, tileSize*y);
			int index = x + (y*numColumns);
			X::TextureId tile = tileMap[index];
			X::DrawSprite(textureIds[tile], position);
		}
	}
}
void DrawGraphLines()
{
	// draw lines between graph nodes
	X::Math::Vector2 offset(tileSize*0.5f, tileSize*0.5f);
	for (size_t y = 0; y < numRows; ++y)
	{
		for (size_t x = 0; x < numColumns; ++x)
		{
			NavNode* node = graph.GetNode(x, y);
			for (size_t n = 0; n < node->neighborCount; ++n)
			{
				X::DrawScreenLine(
					node->position + offset,
					node->neighbors[n]->position + offset,
					X::Math::Vector4::Orange()
					//X::Math::Vector4(x*0.3,((1-x)+y)*0.3, ((1 - y) + x)*0.3,1.0f)
				);
			}
		}
	}
}
void DrawPathPoints()
{
	if (startX != -1 && startY != -1)
	{
		X::Math::Vector2 position(tileSize*startX, tileSize*startY);
		X::DrawSprite(startPointId, position);
	}
	if (endX != -1 && endY != -1)
	{
		X::Math::Vector2 position(tileSize*endX, tileSize*endY);
		X::DrawSprite(endPointId, position);
	}
}
void PlacePathPoints()
{
	float tileSizeOverTwo = 1 / tileSize;
	if (X::IsMouseDown(X::Mouse::LBUTTON))
	{
		int clickedX = static_cast<int>(X::GetMouseScreenX() * tileSizeOverTwo);
		int clickedY = static_cast<int>(X::GetMouseScreenY() * tileSizeOverTwo);

		if (clickedX >= 0 && clickedX < numColumns && clickedY >= 0 && clickedY < numRows)
		{
			startX = clickedX;
			startY = clickedY;
		}
	}
	if (X::IsMouseDown(X::Mouse::RBUTTON))
	{
		int clickedX = static_cast<int>(X::GetMouseScreenX() * tileSizeOverTwo);
		int clickedY = static_cast<int>(X::GetMouseScreenY() * tileSizeOverTwo);

		if (clickedX >= 0 && clickedX < numColumns && clickedY >= 0 && clickedY < numRows)
		{
			endX = clickedX;
			endY = clickedY;
		}
	}
}

void DrawClosedList()
{
	std::list<Ai::Graph<24, 32>::Node*> closedList = graph.GetClosedList();
	if (closedList.size() < 2)
	{
		return;
	}

	X::Math::Vector2 offset(tileSize*0.5f, tileSize*0.5f);
	auto end = closedList.begin();
	auto start = end++;
	for (; end != closedList.end(); ++start, ++end)
	{
		X::DrawScreenLine(
			(*end)->position + offset,
			(*end)->parent->position + offset,
			X::Math::Vector4::Green()
		);
	}
}

void DrawPath()
{
	if (path.size() < 2)
	{
		return;
	}

	X::Math::Vector2 offset(tileSize*0.5f, tileSize*0.5f);
	auto end = path.begin();
	auto start = end++;
	for (; end != path.end(); ++start, ++end)
	{
		X::DrawScreenLine(
			(*start)->position + offset,
			(*end)->position + offset,
			X::Math::Vector4::Cyan()
		);
	}
}

void RunPathSearch()
{
	if (X::IsKeyPressed(X::Keys::O))
	{
		searchMethod = SearchType::DFS;
	}
	if (X::IsKeyPressed(X::Keys::P))
	{
		searchMethod = SearchType::BFS;
	}
	if (startX == -1 || startY == -1 || endX == -1 || endY == -1)
	{
		return;
	}
	if (X::IsMouseDown(X::Mouse::RBUTTON) || X::IsMouseDown(X::Mouse::LBUTTON))
	{
		switch (searchMethod)
		{
		case SearchType::DFS:
		{
			if (graph.RunDFS(startX, startY, endX, endY))
			{
				path = graph.GetPath();
			}
			break;
		}
		case SearchType::BFS:
		{
			if (graph.RunBFS(startX, startY, endX, endY))
			{
				path = graph.GetPath();
			}
			break;
		}
		}

	}
}


bool GameLoop(float deltaTime)
{
	PlacePathPoints();
	RunPathSearch();

	DrawMapTiles();
	//DrawGraphLines();
	DrawPathPoints();
	DrawClosedList();
	DrawPath();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	for (size_t i = 0; i < std::size(textureNames); ++i)
	{
		textureIds[i] = X::LoadTexture(textureNames[i]);
	}
	startPointId = X::LoadTexture("link.png");
	endPointId = X::LoadTexture("carrot.png");
	graph.Initialize(tileSize);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}