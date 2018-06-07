#pragma once

#include "Service.h"

#include <Core\Inc\RTTI.h>

namespace GameEngine
{

class World;
class AABoxColliderComponent;

class CollisionService : public Service
{
	using Subscribers = std::vector<AABoxColliderComponent*>;
	Subscribers mSubscribers;

public:
	REGISTER_TYPE(BSES) // (B)a(seS)ervice

	CollisionService();
	~CollisionService() override;

	CollisionService(const CollisionService&) = delete;
	CollisionService& operator=(const CollisionService&) = delete;

	void Initialize() override;
	void Terminate() override;

	void Update(float dTime) override;

	void Register(AABoxColliderComponent* component);
	void Unregister(AABoxColliderComponent* component);

}; // class CollisionService

} // namespace GameEngine