#include "SCV.h"

SCV::SCV(AI::World& world)
	: AI::Agent(world)
	, mSteeringModule(nullptr)
	, mSeekBehavior(nullptr)
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

	mMaxSpeed = 250.0f;

	mSteeringModule = new AI::SteeringModule(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mSeekBehavior->SetActive(true);
}

void SCV::Unload()
{
	mSteeringModule->Purge();
	X::SafeDelete(mSteeringModule);
}

void SCV::Update(float deltaTime)
{
	X::Math::Vector2 force = mSteeringModule->Calculate();
	X::Math::Vector2 accel = force / 1.0f;	// This should be mass
	mVelocity += accel * deltaTime;
	mPosition += mVelocity * deltaTime;
	
	if (X::Math::MagnitudeSqr(mVelocity) > 0.1f)
	{
		mHeading = X::Math::Normalize(mVelocity);
	}
}

void SCV::Draw()
{
	float angle = atan2(-mHeading.x, mHeading.y) + X::Math::kPi;
	int frame = (int)(angle / X::Math::kTwoPi * 16.0f) % 16;
	X::DrawSprite(mTextureIds[frame], mPosition - X::Math::Vector2(32.0f, 32.0));
}