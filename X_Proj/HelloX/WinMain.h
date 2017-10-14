#pragma once
#include <XEngine.h>

namespace HelloX
{

X::TextureId scvTextureIds[16];
X::Math::Vector2 scvPosition;
float scvAngle = 0.0f;

bool GameLoop(float deltaTime);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::Start();
	for (int i = 0; i < 16; ++i)
	{
		char buffer[128];
		sprintf_s(buffer, "scv_%02d.png", i + 1);
		scvTextureIds[i] = X::LoadTexture(buffer);
	}

	X::Run(GameLoop);
	X::Stop();
	return 0;
}

}