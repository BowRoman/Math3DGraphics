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

	template<class NewStateType/*, std::enable_if<std::is_base_of<StateType, NewStateType>::value>*/>
	void AddState();
	void Purge();

	void Update(float deltaTime);
	void ChangeState(uint32_t stateIndex);

private:
	AgentType& mAgent;
	StateType* mCurrentState;
	std::vector<StateType*> mStates;

}; // class State

template<class AgentType>
StateMachine<AgentType>::StateMachine(AgentType& agent)
	: mAgent(agent)
	, mCurrentState(nullptr)
{
}

template<class AgentType>
StateMachine<AgentType>::~StateMachine()
{
	XASSERT(mStates.empty(), "[StateMachine] Purge must be called before destruction.");

}

template<class AgentType>
template<class NewStateType/*, std::enable_if<std::is_base_of<StateMachine<AgentType>::StateType, NewStateType>::value>*/>
void StateMachine<AgentType>::AddState()
{
	mStates.push_back(new NewStateType());

} // void StateMachine<AgentType>::AddState()

template<class AgentType>
void StateMachine<AgentType>::Purge()
{
	for (auto& state : mStates)
	{
		X::SafeDelete(state);
	}
	mStates.clear();

} // void StateMachine<AgentType>::Purge()

template<class AgentType>
void StateMachine<AgentType>::Update(float deltaTime)
{
	XASSERT(mCurrentState != nullptr, "[StateMachine] mCurrentState cannot be null when Update is called.");
	mCurrentState->Update(mAgent, deltaTime);

} // void StateMachine<AgentType>::Update(float deltaTime)

template<class AgentType>
void StateMachine<AgentType>::ChangeState(uint32_t stateIndex)
{
	XASSERT(stateIndex < (uint32_t)mStates.size(), "[StateMachine] Invalid index: %d.", stateIndex);
	if (mCurrentState)
	{
		mCurrentState->Exit(mAgent);
	}
	mCurrentState = mStates[stateIndex];
	mCurrentState->Enter(mAgent);

} // void StateMachine<AgentType>::Update(float deltaTime)

} // namespace Ai

#endif // #ifndef INCLUDED_AI_STATEMACHINE_H