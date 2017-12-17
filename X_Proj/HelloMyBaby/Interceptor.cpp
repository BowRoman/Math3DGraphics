#include "Interceptor.h"
#include "Food.h"


namespace
{

float huntDist = 300.0f;
float FOV = 0.5f;

class WanderGoal : public Ai::Goal<Interceptor>
{
public:
	WanderGoal(Interceptor& agent)
		: Goal(agent)
	{
	}
	Status Process() override
	{
		X::DrawScreenText("Wandering", mAgent.Position().x, mAgent.Position().y, 20.0f, X::Math::Vector4::Green());
		ActivateIfInactive();
		if (X::Math::DistanceSqr(mAgent.Position(), mAgent.Destination()) <= X::Math::Sqr(32.0f))
		{
			mStatus = Status::Inactive;
		}
		return mStatus;
	}

	void Terminate() override
	{
		mStatus = Status::Inactive;
		mAgent.mSeekBehavior->SetActive(false);
	}

private:
	void Activate() override
	{
		X::Math::Vector2 dest = X::RandomVector2();
		dest.y *= X::GetScreenHeight() - 1.0f;
		dest.x *= X::GetScreenWidth() - 1.0f;
		mAgent.SetDestination(dest);
		mAgent.mSeekBehavior->SetActive(true);
		mStatus = Status::Active;
	}
}; // class WanderGoal : public Ai::Goal<Interceptor>

class HuntGoal : public Ai::Goal<Interceptor>
{
public:
	HuntGoal(Interceptor& agent,int targetIdx)
		: Goal(agent)
		, mTargetIdx(targetIdx)
	{
	}
	Status Process() override
	{
		ActivateIfInactive();

		X::DrawScreenText("Hunting", mAgent.Position().x, mAgent.Position().y, 20.0f, X::Math::Vector4::Green());
		if (!mAgent.mFoodContainer->At(mTargetIdx) || !mAgent.mFoodContainer->At(mTargetIdx)->active)
		{
			mStatus = Status::Inactive;
		}
		else if (X::Math::DistanceSqr(mAgent.Position(), mAgent.Destination()) <= X::Math::Sqr(32.0f))
		{
			mAgent.mFoodContainer->At(mTargetIdx)->active = false;
			mStatus = Status::Inactive;
		}
		return mStatus;
	}

	void Terminate() override
	{
		mStatus = Status::Inactive;
		mAgent.mSeekBehavior->SetActive(false);
	}

private:
	void Activate() override
	{
		mAgent.SetDestination(mAgent.mFoodContainer->At(mTargetIdx)->mPosition);
		mAgent.mSeekBehavior->SetActive(true);
		mStatus = Status::Active;
	}
	int mTargetIdx;
}; // class WanderGoal : public Ai::Goal<Interceptor>

class IdleStrategy : public Ai::Strategy<Interceptor>
{
public:
	IdleStrategy(Interceptor& agent)
		: Strategy(agent)
	{}
	float CalculateDesirability()
	{
		return 0.01f;
	}
	Ai::Goal<Interceptor>* CreateGoal()
	{
		WanderGoal* goal = new WanderGoal(mAgent);
		return goal;
	}
}; // class SurviveStrategy : public Ai::Strategy<Interceptor>

class HuntStrategy : public Ai::Strategy<Interceptor>
{
public:
	HuntStrategy(Interceptor& agent, float huntDistance)
		: Strategy(agent)
		, mHuntDistance{ huntDistance }
	{}
	float CalculateDesirability()
	{
		auto allFood = mAgent.mFoodContainer->GetAllActiveFood();
		int nearestFoodIdx = -1;
		for (int i = 0; i < allFood.size(); ++i)
		{
			auto food = mAgent.mFoodContainer->At(i);
			if (food)
			{
				float dist = (X::Math::DistanceSqr(mAgent.Position(), food->mPosition));
				auto direction = X::Math::Normalize(food->mPosition - mAgent.Position());
				auto heading = X::Math::Normalize(mAgent.Destination() - mAgent.Position());
				float x = X::Math::Dot(direction, heading);
				if (dist < (X::Math::Sqr(mHuntDistance)) && x >= FOV )
				{
					if (nearestFoodIdx == -1 || dist < (X::Math::DistanceSqr(mAgent.Position(), mAgent.mFoodContainer->At(nearestFoodIdx)->mPosition)))
					{
						nearestFoodIdx = i;
					}
				}
			}
		}
		if (nearestFoodIdx != -1)
		{
			mTargetIdx = nearestFoodIdx;

			if (mAgent.mFoodContainer->At(nearestFoodIdx)->active)
			{
				return X::Math::DistanceSqr(mAgent.Position(), mAgent.mFoodContainer->At(nearestFoodIdx)->mPosition) / mHuntDistance;
			}
			else
			{
				return 0.0f;
			}
		}
		else
		{
			return 0.0f;
		}
	}
	Ai::Goal<Interceptor>* CreateGoal()
	{
		HuntGoal* goal = new HuntGoal(mAgent, mTargetIdx);
		return goal;
	}
private:
	float mHuntDistance;
	int mTargetIdx;

}; // class SurviveStrategy : public Ai::Strategy<Interceptor>

class FOV : public Ai::Sensor
{
public:
	FOV() {}

	void Update(Ai::Agent& agent, Ai::MemoryRecords& memory, float deltaTime) override
	{

	}
	const char* GetName() const override
	{

	}
}; // class FOV


} // namespace

Interceptor::Interceptor(Ai::World& world, FoodContainer* foodContainer)
	: Ai::Agent(world)
	, mFoodContainer(foodContainer)
	, mSteeringModule(nullptr)
	, mSeekBehavior(nullptr)
	, mSeparationBehavior(nullptr)
	, mDecisionModule(nullptr)
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
		sprintf_s(buffer, "Interceptor_%02d.png", static_cast<int>(i + 1));
		mTextureIds[i] = X::LoadTexture(buffer);
	}

	mMaxSpeed = 250.0f;

	mSteeringModule = new Ai::SteeringModule(*this);
	mSeparationBehavior = mSteeringModule->AddBehavior<Ai::SeparationBehavior>();
	mSeparationBehavior->SetActive(true);
	mSeekBehavior = mSteeringModule->AddBehavior<Ai::SeekBehavior>();

	mDecisionModule = new Ai::DecisionModule<Interceptor>(*this);
	mDecisionModule->AddStrategy(new IdleStrategy(*this));
	mDecisionModule->AddStrategy(new HuntStrategy(*this, huntDist));
}

void Interceptor::Unload()
{
	mSteeringModule->Purge();
	X::SafeDelete(mSteeringModule);

	mDecisionModule->Purge();
	X::SafeDelete(mDecisionModule);
}

void Interceptor::Update(float deltaTime)
{
	mDecisionModule->Update();
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
	auto pos = mPosition - X::Math::Vector2(16.0f, 16.0);
	X::DrawScreenCircle(pos, huntDist*0.6f, X::Math::Vector4::White());
	auto left = X::Math::Rotate((mHeading*huntDist*0.6f), FOV);
	auto right = X::Math::Rotate((mHeading*huntDist*0.6f), -FOV);
	X::DrawScreenLine(pos, pos + left, X::Math::Vector4::White());
	X::DrawScreenLine(pos, pos + right, X::Math::Vector4::White());
}
