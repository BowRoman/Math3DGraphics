#ifndef INCLUDED_INTERCEPTOR_H
#define INCLUDED_INTERCEPTOR_H

#include <AI.h>
#include <XEngine.h>

class Interceptor : public Ai::Agent
{
public:
	Interceptor(Ai::World& world);
	~Interceptor();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Draw();

private:
	Ai::SteeringModule* mSteeringModule;
	//Ai::SeekBehavior* mSeekBehavior;
	Ai::SeparationBehavior* mSeparationBehavior;
	X::TextureId mTextureIds[32];
};

#endif // #ifndef INCLUDED_INTERCEPTOR_H