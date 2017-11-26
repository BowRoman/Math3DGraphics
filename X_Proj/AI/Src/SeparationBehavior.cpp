#include "SeparationBehavior.h"

#include "Agent.h"
#include "World.h"

using namespace Ai;

X::Math::Vector2 SeparationBehavior::Calculate(Agent& agent)
{
	float radius = 200.0f;
	X::Math::Circle range(agent.Position(), radius);
	auto neighborhood = agent.GetWorld().GetNeighborhood(range);

	X::Math::Vector2 separation;
	for (auto neighbor : neighborhood)
	{
		if (neighbor == &agent)
		{
			continue;
		}

		X::Math::Vector2 neighborToAgent = agent.Position() - neighbor->Position();
		float distanctToAgent = X::Math::Magnitude(neighborToAgent);
		separation += (neighborToAgent / distanctToAgent) * (radius - distanctToAgent);
	}
	return separation;
}