#include "Agent.h"
#include "World.h"

using namespace Ai;

Agent::Agent(World& world)
	: mWorld(world)
	, mMaxSpeed{ 1.0f }
	, mMemorySpan{ 5.0f }
{
	mWorld.RegisterAgent(this);

} // Agent::Agent(World& world)

Agent::~Agent()
{
	mWorld.UnregisterAgent(this);

} // Agent::~Agent()

X::Math::Matrix3 Agent::LocalToWorld() const
{
	return X::Math::Matrix3(
		mHeading.y, -mHeading.x, 0.0f,
		mHeading.x, mHeading.y, 0.0f,
		mPosition.x, mPosition.y, 1.0f
	);

} // X::Math::Matrix3 Agent::LocalToWorld() const
