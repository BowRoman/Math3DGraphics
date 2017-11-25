#include "SCV.h"
#include "Constant.h"

class HarvestState : public Ai::State<SCV>
{
public:
	virtual void Enter(SCV& agent) override
	{
		agent.SetDestination(kResourceLocation);
	}
	virtual void Update(SCV& agent, float deltaTime) override
	{
		if (X::Math::Distance(agent.GetPosition(), kResourceLocation) < 16.0f)
		{
			agent.SetDestination(agent.GetPosition());
			agent.mCurrentResources += agent.mResourceGatherRate*deltaTime;
			if (agent.mCurrentResources >= agent.mMaxResources)
			{
				agent.ChangeState(SCV::State::Deposit);
			}
		}
	}
	virtual void Exit(SCV& agent) override
	{

	}
};

class DepositState : public Ai::State<SCV>
{
public:
	virtual void Enter(SCV& agent) override
	{
		agent.SetDestination(kDepositLocation);
	}
	virtual void Update(SCV& agent, float deltaTime) override
	{
		if (X::Math::Distance(agent.GetPosition(), kDepositLocation) < 10.0f)
		{
			agent.mCurrentResources = 0.0f;
			agent.ChangeState(SCV::State::Harvest);
		}
	}
	virtual void Exit(SCV& agent) override
	{

	}
};

SCV::SCV()
	: mCurrentResources(0.0f)
	, mMaxResources(3.0f)
	, mResourceGatherRate(1.0f)
	, mAngle(0.0f)
	, mStateMachine(nullptr)
{
}

SCV::~SCV()
{
	if (mStateMachine)
	{
		mStateMachine->Purge();
		X::SafeDelete(mStateMachine);
	}
}

void SCV::Load()
{
	for (int i = 0; i < 16; ++i)
	{
		char buffer[128];
		sprintf_s(buffer, "scv_%02d.png", i + 1);
		mTextureIds[i] = X::LoadTexture(buffer);
	}
	mResourceTextureId = X::LoadTexture("bullet2.png");

	mStateMachine = new Ai::StateMachine<SCV>(*this);
	mStateMachine->AddState<HarvestState>();
	mStateMachine->AddState<DepositState>();
	mStateMachine->ChangeState(uint32_t(State::Harvest));
}

void SCV::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
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
	X::DrawSprite(mTextureIds[frame], mPosition - X::Math::Vector2(32.0f, 32.0f));
	if (mCurrentResources >= 1.0f)
	{
		X::DrawSprite(mResourceTextureId, mPosition - X::Math::Vector2(32.0f, 16.0f));
		if (mCurrentResources >= 2.0f)
		{
			X::DrawSprite(mResourceTextureId, mPosition - X::Math::Vector2(16.0f, 16.0f));
			if (mCurrentResources >= 3.0f)
			{
				X::DrawSprite(mResourceTextureId, mPosition - X::Math::Vector2(0.0f, 16.0f));
			}
		}
	}
}

void SCV::ChangeState(State state)
{
	mStateMachine->ChangeState(uint32_t(state));
}