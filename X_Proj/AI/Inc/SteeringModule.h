#ifndef INCLUDED_AI_STEERINGMODULE_H
#define INCLUDED_AI_STEERINGMODULE_H

#include "SteeringBehavior.h"

namespace Ai
{

class SteeringModule
{
public:
	SteeringModule(Agent& agent);
	~SteeringModule();

	template <class SteeringBehaviorType>
	SteeringBehaviorType* AddBehavior();
	void Purge();

	X::Math::Vector2 Calculate();

private:
	Agent& mAgent;
	std::vector<SteeringBehavior*> mBehaviors;

};

template <class SteeringBehaviorType>
SteeringBehaviorType* SteeringModule::AddBehavior()
{
	static_assert(std::is_base_of<SteeringBehavior, SteeringBehaviorType>::value, "[SteeringModule] Can only add SteeringBehavior types.");
	auto behavior = new SteeringBehaviorType();
	mBehaviors.push_back(behavior);
	return behavior;

}

} // namespace AI

#endif // #ifndef INCLUDED_AI_STEERINGMODULE_H
