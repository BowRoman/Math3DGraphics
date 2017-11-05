#include "Constant.h"
#include "SCV.h"

SCV scv;
X::TextureId refineryId;
X::TextureId mineralId;

bool GameLoop(float deltaTime)
{
	X::DrawSprite(refineryId, refineryLocation);
	X::DrawSprite(mineralId, mineralLocation);

	scv.Update(deltaTime);
	scv.Draw();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	scv.Load();
	refineryId = X::LoadTexture("refinery_05.png");
	mineralId = X::LoadTexture("minerals.png");

	X::Run(GameLoop);

	X::Stop();
	return 0;
}