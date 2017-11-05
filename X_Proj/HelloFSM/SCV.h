#pragma once
#include <XEngine.h>

class SCV
{
public:
	SCV();
	~SCV();

	void Load();
	void Update(float deltaTime);
	void Draw();

	void SetPosition(const X::Math::Vector2& pos) { mPosition = pos; }
	void SetDestination(const X::Math::Vector2& dest) { mDestination = dest; }

	const X::Math::Vector2& GetPosition() const { return mPosition; }
private:
	X::TextureId mTextureIds[16];
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mDestination;
	float mAngle;
};