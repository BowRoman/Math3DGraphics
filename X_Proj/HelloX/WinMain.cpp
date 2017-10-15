#include <XEngine.h>

X::TextureId scvTextureIds[16];
X::Math::Vector2 scvPosition;
float scvAngle = 0.0f;

bool GameLoop(float deltaTime)
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();
	X::Math::Vector2 destination(mouseX, mouseY);

	if (X::Math::Distance(scvPosition, destination) > 5.0f)
	{
		X::Math::Vector2 direction = X::Math::Normalize(destination - scvPosition);
		scvPosition += direction * 100.0f * deltaTime;
		scvAngle = atan2(-direction.x, direction.y) + X::Math::kPi;
	}

	int frame = (int)(scvAngle / X::Math::kTwoPi * 16.0f) % 16;
	X::DrawSprite(scvTextureIds[frame], scvPosition - X::Math::Vector2(32.0f, 32.0));

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

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