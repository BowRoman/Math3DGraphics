#include "SteeringModule.h"

using namespace Ai;

SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{}

SteeringModule::~SteeringModule()
{
	XASSERT(mBehaviors.empty(), "[SteeringModule] Purge() must be called before destruction.");
}

void SteeringModule::Purge()
{
	for (auto b : mBehaviors)
	{
		X::SafeDelete(b);
	}
	mBehaviors.clear();
}

X::Math::Vector2 SteeringModule::Calculate()
{
	X::Math::Vector2 total;
	for (auto b : mBehaviors)
	{
		if (b->IsActive())
		{
			total += b->Calculate(mAgent);
		}
	}

	return total;
}