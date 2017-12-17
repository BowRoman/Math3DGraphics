#include "SeparationBehavior.h"

#include "Agent.h"
#include "World.h"

using namespace Ai;

X::Math::Vector2 SeparationBehavior::Calculate(Agent& agent)
{
	float radius = 200.0f;
	X::Math::Circle range(agent.Position(), radius);
	auto neighborhood = agent.GetWorld().GetNeighborhood(range);

	X::Math::Vector2 separation = X::Math::Vector2::Zero();
	for (auto neighbor : neighborhood)
	{
		if (neighbor == &agent)
		{
			continue;
		}

		X::Math::Vector2 neighborToAgent = agent.Position() - neighbor->Position();
		float distanceToAgent = X::Math::Magnitude(neighborToAgent);
		separation += (neighborToAgent / distanceToAgent) * X::Math::Min((radius - distanceToAgent), agent.MaxSpeed());
	}
	return separation;
}