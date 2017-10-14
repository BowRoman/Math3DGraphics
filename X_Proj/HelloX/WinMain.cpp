#include "WinMain.h"

bool HelloX::GameLoop(float deltaTime)
{
	float mouseX = static_cast<float>(X::GetMouseScreenX());
	float mouseY = static_cast<float>(X::GetMouseScreenY());
	X::Math::Vector2 destination(mouseX, mouseY);

	if (X::Math::Distance(scvPosition, destination) > 5.0f)
	{
		X::Math::Vector2 direction = X::Math::Normalize(destination - scvPosition);
		scvPosition += direction * 100.0f * deltaTime;
		scvAngle = atan2(-direction.x, direction.y) + X::Math::kPi;
	}

	int frame = static_cast<int>(scvAngle / X::Math::kTwoPi * 16.0f) % 16;
	X::DrawSprite(static_cast<int>(scvTextureIds[frame]), scvPosition - X::Math::Vector2(32.0f, 32.0));

	return X::IsKeyPressed(X::Keys::ESCAPE);
}