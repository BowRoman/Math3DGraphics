#include "Precompiled.h"
#include "GameObjectFactory.h"


GameObjectFactory::GameObjectFactory(GameObjectAllocator& allocator)
	: mGameObjectAllocator(allocator)
{
}

GameObject* GameObjectFactory::Create(const char* templateFileName)
{
	GameObject* gameObject = mGameObjectAllocator.New();

	// Homework:
	// Read the template file using TinyXML
	// Then add the components to the gameobject and
	// inject the correct arguments to the components

	return gameObject;
}

void GameObjectFactory::Destroy(GameObject* gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}