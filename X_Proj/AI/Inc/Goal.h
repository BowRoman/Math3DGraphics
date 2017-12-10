#ifndef INCLUDED_AI_GOAL_H
#define INCLUDED_AI_GOAL_H

namespace Ai
{

template <typename AgentType>
class Goal
{
public:
	enum class Status
	{
		Inactive,
		Active,
		Completed,
		Failed
	};

	Goal(AgentType& agent);
	virtual ~Goal();

	// Function to update this action
	virtual Status Process() = 0;

	// Function to terminate this action
	virtual void Terminate() = 0;

protected:
	// Function to activate this action
	virtual void Activate() = 0;
	void ActivateIfInactive();

	AgentType& mAgent;
	Status mStatus;
};

#include "Goal.inl"

} // namespace Ai

#endif // #ifndef INCLUDED_AI_GOAL_H