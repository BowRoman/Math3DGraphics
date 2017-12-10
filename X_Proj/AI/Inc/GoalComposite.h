#ifndef INCLUDED_AI_GOALCOMPOSITE_H
#define INCLUDED_AI_GOALCOMPOSITE_H

#include "Goal.h"

namespace Ai
{

template <typename AgentType>
class GoalComposite : public Goal<AgentType>
{
public:
	GoalComposite(AgentType& agent);
	virtual ~GoalComposite();

protected:
	void AddSubgoal(Goal<AgentType>* goal);
	void RemoveAllSubgoals();
	Status ProcessSubgoals();

	std::list<Goal<AgentType>*> mSubgoals;
};

#include "GoalComposite.inl"

} // namespace Ai

#endif // #ifndef INCLUDED_AI_GOALCOMPOSITE_H