#ifndef INCLUDED_SCV_H
#define INCLUDED_SCV_H

#include <AI.h>
#include <XEngine.h>

class SCV : public AI::Agent
{
public:
	SCV(AI::World& world);
	~SCV();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Draw();

private:
	AI::SteeringModule* mSteeringModule;
	AI::SeekBehavior* mSeekBehavior;
	X::TextureId mTextureIds[16];
};

#endif // #ifndef INCLUDED_SCV_H