#ifndef INCLUDED_SCV_H
#define INCLUDED_SCV_H

#include <AI.h>
#include <XEngine.h>

class SCV
{
public:
	enum State
	{
		GoToMineral,
		GoToRefinery
	};

	SCV();
	~SCV();

	void Load();
	void Update(float deltaTime);
	void Draw();

	void SetPosition(const X::Math::Vector2& pos)		{ mPosition = pos; }
	void SetDestination(const X::Math::Vector2& dest)	{ mDestination = dest; }

	const X::Math::Vector2& GetPosition() const			{ return mPosition; }

	void ChangeState(State state);

private:
	AI::StateMachine<SCV>* mStateMachine;

	X::TextureId mTextureIds[16];
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mDestination;
	float mAngle;
};

#endif // #ifndef INCLUDED_SCV_H