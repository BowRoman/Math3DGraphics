#pragma once
#include <XEngine.h>
#include <Ai.h>

class SCV
{
public:
	enum class State
	{
		Harvest,
		Deposit
	};

	SCV();
	~SCV();

	void Load();
	void Update(float deltaTime);
	void Draw();

	void SetPosition(const X::Math::Vector2& pos) { mPosition = pos; }
	void SetDestination(const X::Math::Vector2& dest) { mDestination = dest; }

	const X::Math::Vector2& GetPosition() const { return mPosition; }

	void ChangeState(State state);

public:
	float mCurrentResources;
	float mMaxResources;
	float mResourceGatherRate;

private:
	X::TextureId mTextureIds[16];
	X::TextureId mResourceTextureId;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mDestination;
	float mAngle;

	Ai::StateMachine<SCV>* mStateMachine;
};