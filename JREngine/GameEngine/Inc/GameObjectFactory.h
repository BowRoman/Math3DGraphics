#pragma once
#include "GameObject.h"

class GameObjectFactory
{
	GameObjectAllocator& mGameObjectAllocator;

public:
	GameObjectFactory(GameObjectAllocator& allocator);

	GameObject* Create(const char* templateFileName);
	void Destroy(GameObject* gameObject);
};