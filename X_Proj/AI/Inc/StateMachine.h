#ifndef INCLUDED_AI_STATEMACHINE_H
#define INCLUDED_AI_STATEMACHINE_H

#include <XEngine.h>

namespace Ai
{

template <class AgentType>
class StateMachine
{
public:
	typedef State<AgentType> StateType;

	StateMachine(AgentType& agent);
	~StateMachine();

	void AddState(StateType* newState);
	void Purge();

	void Update(float deltaTime);
	void ChangeState(uint32_t index);

}; // class State

}

#endif // #ifndef INCLUDED_AI_STATEMACHINE_H