#include <Ai.h>
#include <XEngine.h>

#include "SCV.h"

SCV scv;

bool GameLoop(float deltaTime)
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();
	scv.SetDestination(X::Math::Vector2(mouseX, mouseY));
	scv.Update(deltaTime);
	scv.Draw();
	
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	scv.Load();
	scv.SetPosition(X::Math::Vector2::Zero());
	X::Run(GameLoop);
	X::Stop();
	return 0;
}