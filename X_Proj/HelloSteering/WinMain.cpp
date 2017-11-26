#include "SCV.h"

Ai::World aiWorld;
SCV scv(aiWorld);

bool GameLoop(float deltaTime)
{
	float destX = (float)X::GetMouseScreenX();
	float destY = (float)X::GetMouseScreenY();
	scv.Destination() = { destX, destY };

	scv.Update(deltaTime);
	scv.Draw();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	scv.Load();

	X::Run(GameLoop);

	X::Stop();
	return 0;
}