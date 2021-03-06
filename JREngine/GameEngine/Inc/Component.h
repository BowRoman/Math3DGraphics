#pragma once

#include <Core\Inc\RTTI.h>

namespace GameEngine
{

class GameObject;

class Component
{
protected:
	friend class GameObject;
	GameObject* mGameObject;

public:
	REGISTER_TYPE(BSEC) // (B)a(seC)omponent

	Component() : mGameObject{ nullptr } {}
	virtual ~Component() {}

	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void Update(float dTime) {}
	virtual void Render() {}
	virtual void Render2D() {}

	GameObject& GetOwner() { return *mGameObject; }
	const GameObject& GetOwner() const { return *mGameObject; }

}; // class Component

} // namespace GameEngine