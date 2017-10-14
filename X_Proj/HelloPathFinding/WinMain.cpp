#include <XEngine.h>

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

const int numRows = 8;
const int numColumns = 10;
const int tileMap[]
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 2, 3, 1, 1, 1, 1, 1, 1, 1,
	1, 4, 5, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 2, 3, 1, 1,
	1, 1, 1, 1, 1, 1, 4, 5, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1
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
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	for (size_t i = 0; i < std::size(textureNames); ++i)
	{
		textureIds[i] = X::LoadTexture(textureNames[i]);
	}
	X::Run(GameLoop);
	X::Stop();
	return 0;
}