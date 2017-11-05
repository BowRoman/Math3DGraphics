#include "SCV.h"

SCV::SCV()
	: mAngle(0.0f)
{
}

SCV::~SCV()
{
}

void SCV::Load()
{
	for (int i = 0; i < 16; ++i)
	{
		char buffer[128];
		sprintf_s(buffer, "scv_%02d.png", i + 1);
		mTextureIds[i] = X::LoadTexture(buffer);
	}
}

void SCV::Update(float deltaTime)
{
	if (X::Math::Distance(mPosition, mDestination) > 5.0f)
	{
		X::Math::Vector2 direction = X::Math::Normalize(mDestination - mPosition);
		mPosition += direction * 100.0f * deltaTime;
		mAngle = atan2(-direction.x, direction.y) + X::Math::kPi;
	}
}

void SCV::Draw()
{
	int frame = (int)(mAngle / X::Math::kTwoPi * 16.0f) % 16;
	X::DrawSprite(mTextureIds[frame], mPosition - X::Math::Vector2(32.0f, 32.0));
}
