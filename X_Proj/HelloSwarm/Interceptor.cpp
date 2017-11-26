#include "Interceptor.h"

Interceptor::Interceptor(Ai::World& world)
	: Ai::Agent(world)
	, mSteeringModule(nullptr)
	//, mSeekBehavior(nullptr)
	, mSeparationBehavior(nullptr)
{
}

Interceptor::~Interceptor()
{
}

void Interceptor::Load()
{
	for (size_t i = 0; i < std::size(mTextureIds); ++i)
	{
		char buffer[128];
		sprintf_s(buffer, "Interceptor_%02d.png", i + 1);
		mTextureIds[i] = X::LoadTexture(buffer);
	}

	mMaxSpeed = 250.0f;

	mSteeringModule = new Ai::SteeringModule(*this);
	mSeparationBehavior = mSteeringModule->AddBehavior<Ai::SeparationBehavior>();
	mSeparationBehavior->SetActive(true);
}

void Interceptor::Unload()
{
	mSteeringModule->Purge();
	X::SafeDelete(mSteeringModule);
}

void Interceptor::Update(float deltaTime)
{
	X::Math::Vector2 force = mSteeringModule->Calculate();
	X::Math::Vector2 accel = force / 1.0f;	// This should be mass
	mVelocity += accel * deltaTime;
	mPosition += mVelocity * deltaTime;
	
	if (X::Math::MagnitudeSqr(mVelocity) > 0.1f)
	{
		mHeading = X::Math::Normalize(mVelocity);
	}

	// Wrapping logic
	float height = static_cast<float>(X::GetScreenHeight());
	auto width = static_cast<float>(X::GetScreenWidth());
	if (mPosition.y < 0.0f) mPosition.y += height;
	if (mPosition.y > height) mPosition.y -= height;
	if (mPosition.x < 0.0f) mPosition.x += width;
	if (mPosition.x > width) mPosition.x -= width;
}

void Interceptor::Draw()
{
	float angle = atan2(-mHeading.x, mHeading.y) + X::Math::kPi;
	int frame = (int)(angle / X::Math::kTwoPi * static_cast<float>(std::size(mTextureIds))) % std::size(mTextureIds);
	X::DrawSprite(mTextureIds[frame], mPosition - X::Math::Vector2(32.0f, 32.0));
}