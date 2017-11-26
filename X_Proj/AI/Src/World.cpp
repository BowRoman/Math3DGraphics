#include "World.h"

using namespace Ai;

World::World()
{
} // World::World()

World::~World()
{
} // World::~World()

void World::DebugRender()
{
	for (auto o : mObstacles)
	{
		X::DrawScreenCircle(o.center, o.radius, X::Math::Vector4::Cyan());
	}
	for (auto w : mWalls)
	{
		X::DrawScreenLine(w.from, w.to, X::Math::Vector4::Cyan());
	}
}

void World::AddObstacles(const X::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);

} // void World::AddObstacles(const X::Math::Circle& obstacle)

void World::AddWall(const X::Math::LineSegment& wall)
{
	mWalls.push_back(wall);

} // void World::AddWall(const X::Math::LineSegment& wall)

AgentList World::GetNeighborhood(const X::Math::Circle& range) const
{
	AgentList agentList;
	for (auto agent : mAgentList)
	{
		if ((X::Math::DistanceSqr(range.center, agent->Position())) < (X::Math::Sqr(range.radius)))
		{
			agentList.push_back(agent);
		}
	}
	return agentList;
} // AgentList World::GetNeighborhood(const X::Math::Circle & range) const

void World::RegisterAgent(Agent* agent)
{
	XASSERT(std::find(mAgentList.begin(), mAgentList.end(), agent) == mAgentList.end(), "[World] Agent cannot be registered twice.");
	mAgentList.push_back(agent);

} // void World::RegisterAgent(Agent* agent)

void World::UnregisterAgent(Agent* agent)
{
	mAgentList.erase(std::find(mAgentList.begin(), mAgentList.end(), agent));

} // void World::UnregisterAgent(Agent* agent)
