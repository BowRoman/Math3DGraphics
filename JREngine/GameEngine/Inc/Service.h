#pragma once

#include <Core\Inc\RTTI.h>

namespace GameEngine
{

class World;

class Service
{
protected:
	friend class World;
	World* mWorld;

public:
	REGISTER_TYPE(BSES) // (B)a(seS)ervice

	Service() : mWorld{ nullptr } {}
	virtual ~Service() {}

	Service(const Service&) = delete;
	Service& operator=(const Service&) = delete;

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void Update(float dTime) {}
	virtual void Render() {}
	virtual void Render2D() {}

	World& GetOwner() { return *mWorld; }
	const World& GetOwner() const { return *mWorld; }

}; // class Service

} // namespace GameEngine