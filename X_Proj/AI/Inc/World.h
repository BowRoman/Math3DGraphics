#ifndef INCLUDED_AI_WORLD_H
#define INCLUDED_AI_WORLD_H

#include "Agent.h"

namespace Ai
{

class World
{
public:
	typedef std::vector<X::Math::Circle> Obstacles;
	typedef std::vector<X::Math::LineSegment> Walls;

	World();
	~World();

	void DebugRender();

	void AddObstacles(const X::Math::Circle& obstacle);
	const Obstacles& GetObstacles() const { return mObstacles; }

	void AddWall(const X::Math::LineSegment& wall);
	const Walls& GetWalls() const { return mWalls; }

	AgentList GetNeighborhood(const X::Math::Circle& range) const;

private:
	friend class Agent;

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

private:
	AgentList mAgentList;
	Obstacles mObstacles;
	Walls mWalls;

}; // class World

} // namespace AI

#endif // #ifndef INCLUDED_AI_WORLD_H