#pragma once

#include <Core\Inc\RTTI.h>

namespace GameEngine
{

class GameObject;

class Component
{
	friend class GameObject;
	GameObject* mGameObject;

public:
	REGISTER_TYPE(BSEC) // (B)a(seC)omponent

	Component();
	virtual ~Component();

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