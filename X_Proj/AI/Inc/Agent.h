#ifndef INCLUDED_AI_AGENT_H
#define INCLUDED_AI_AGENT_H

#include <XEngine.h>

namespace Ai
{

class World;

class Agent
{
public:
	Agent(World& world);
	virtual ~Agent();

	X::Math::Matrix3 LocalToWorld() const;

	World& GetWorld() const						{ return mWorld; }
	
	X::Math::Vector2& Position()				{ return mPosition; }
	const X::Math::Vector2& Position() const	{ return mPosition; }
	
	X::Math::Vector2& Velocity()				{ return mVelocity; }
	const X::Math::Vector2& Velocity() const	{ return mVelocity; }

	X::Math::Vector2& Destination()				{ return mDestination; }
	const X::Math::Vector2& Destination() const { return mDestination; }
	void SetDestination(X::Math::Vector2 dest)  { mDestination = dest; }

	X::Math::Vector2& Heading()					{ return mHeading; }
	const X::Math::Vector2& Heading() const		{ return mHeading; }

	float& MaxSpeed()							{ return mMaxSpeed; }
	float MaxSpeed() const						{ return mMaxSpeed; }

	float& MemorySpan()							{ return mMemorySpan; }
	float MemorySpan() const					{ return mMemorySpan; }

protected:
	World& mWorld;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mVelocity;
	X::Math::Vector2 mDestination;
	X::Math::Vector2 mHeading;
	float mMaxSpeed;
	float mMemorySpan;
}; // class Agent

using AgentList = std::vector<Agent*>;

} // namespace AI

#endif // #ifndef INCLUDED_AI_AGENT_H