#ifndef INCLUDED_INTERCEPTOR_H
#define INCLUDED_INTERCEPTOR_H

#include <AI.h>
#include <XEngine.h>
class FoodContainer;

class Interceptor : public Ai::Agent
{
public:
	Interceptor(Ai::World& world, FoodContainer* foodContainer);
	~Interceptor();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Draw();

	Ai::SteeringModule* mSteeringModule;
	Ai::SeekBehavior* mSeekBehavior;
	Ai::SeparationBehavior* mSeparationBehavior;

	Ai::PerceptionModule* mPerceptionModule;
	Ai::MemoryRecords mMemoryRecord;

	Ai::DecisionModule<Interceptor>* mDecisionModule;
	X::TextureId mTextureIds[32];
	FoodContainer* mFoodContainer;
};

#endif // #ifndef INCLUDED_INTERCEPTOR_H