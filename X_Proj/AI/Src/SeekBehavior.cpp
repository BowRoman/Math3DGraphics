#include "SeekBehavior.h"

#include "Agent.h"

using namespace Ai;

X::Math::Vector2 SeekBehavior::Calculate(Agent& agent)
{
	X::Math::Vector2 desiredDirection = X::Math::Normalize(agent.Destination() - agent.Position());
	X::Math::Vector2 desiredVelocity = desiredDirection * agent.MaxSpeed();
	return desiredVelocity - agent.Velocity();
}