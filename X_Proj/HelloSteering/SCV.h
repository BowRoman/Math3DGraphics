#ifndef INCLUDED_SCV_H
#define INCLUDED_SCV_H

#include <AI.h>
#include <XEngine.h>

class SCV : public Ai::Agent
{
public:
	SCV(Ai::World& world);
	~SCV();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Draw();

private:
	Ai::SteeringModule* mSteeringModule;
	Ai::SeekBehavior* mSeekBehavior;
	X::TextureId mTextureIds[16];
};

#endif // #ifndef INCLUDED_SCV_H