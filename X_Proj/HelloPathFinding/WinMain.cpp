#include <Ai.h>
#include <XEngine.h>

const size_t numRows = 8;
const size_t numColumns = 10;
typedef Ai::Graph<numRows, numColumns> NavGraph;
typedef Ai::Graph<numRows, numColumns>::Node NavNode;

enum class TileType
{
	None	= 0,
	Grass	= 1,
	Flower	= 2,
	TreeTL	= 3,
	TreeTR	= 4,
	TreeBL	= 5,
	TreeBR	= 6
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

const float tileSize = 32.0f;
NavGraph graph;
const size_t tileMap[]
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 4, 1, 2, 1, 1, 2, 1, 1,
	1, 5, 6, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 3, 4, 1, 1,
	1, 1, 1, 1, 1, 1, 5, 6, 1, 1,
	1, 1, 2, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 3, 4, 1,
	1, 1, 1, 1, 1, 1, 1, 5, 6, 1
};

bool GameLoop(float deltaTime)
{
	for (int y = 0; y < numRows; ++y)
	{
		for (int x = 0; x < numColumns; ++x)
		{
			X::Math::Vector2 position(32.0f*x, 32.0f*y);
			int index = x + (y*numColumns);
			int tile = tileMap[index];
			X::DrawSprite(textureIds[tile], position);
		}
	}

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
					X::Math::Vector4::Cyan()
				);
			}
		}
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	for (size_t i = 0; i < std::size(textureNames); ++i)
	{
		textureIds[i] = X::LoadTexture(textureNames[i]);
	}
	graph.Initialize(tileSize);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}