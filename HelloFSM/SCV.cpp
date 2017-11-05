#include "SCV.h"
#include "Constant.h"

class GoToMineralState : public AI::State<SCV>
{
public:
	void Enter(SCV& agent)
	{
		agent.SetDestination(mineralLocation);
	}

	void Update(SCV& agent, float deltaTime)
	{
		if (X::Math::Distance(agent.GetPosition(), mineralLocation) < 10.0f)
		{
			agent.ChangeState(SCV::GoToRefinery);
		}
	}

	void Exit(SCV& agent)
	{
	}
};

class GoToRefineryState : public AI::State<SCV>
{
public:
	void Enter(SCV& agent)
	{
		agent.SetDestination(refineryLocation);
	}

	void Update(SCV& agent, float deltaTime)
	{
		if (X::Math::Distance(agent.GetPosition(), refineryLocation) < 10.0f)
		{
			agent.ChangeState(SCV::GoToMineral);
		}
	}

	void Exit(SCV& agent)
	{
	}
};

SCV::SCV()
	: mStateMachine(nullptr)
	, mAngle(0.0f)
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

	mStateMachine = new AI::StateMachine<SCV>(*this);
	mStateMachine->AddState<GoToMineralState>();
	mStateMachine->AddState<GoToRefineryState>();
	mStateMachine->ChangeState(GoToMineral);
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
	X::DrawSprite(mTextureIds[frame], mPosition - X::Math::Vector2(32.0f, 32.0));
}

void SCV::ChangeState(State state)
{
	mStateMachine->ChangeState(state);
}