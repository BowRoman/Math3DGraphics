#include "Precompiled.h"
#include "World.h"

void World::Initialize(uint32_t capacity)
{
	mGameObjectAllocator = std::unique_ptr<GameObjectAllocator>(new GameObjectAllocator(capacity));
	mGameObjectVector.reserve(capacity);
}

void World::Terminate()
{
	for (auto obj : mGameObjectVector)
	{
		obj->Terminate();
		Destroy(obj);
	}
}

void World::LoadLevel(const char* levelFileName)
{
}

GameObject* World::Create(const char* templateFileName, const char* name)
{
	GameObject* object = mGameObjectFactory->Create(templateFileName);
	if (object)
	{
		mGameObjectVector.push_back(object);
	}
	return object;
}

GameObject* World::Find(const char* name)
{
	return nullptr;
}

void World::Destroy(GameObject* gameObj)
{
	mGameObjectFactory->Destroy(gameObj);
}

void World::Update(float deltaTime)
{

}

void World::Render()
{

}

void World::Render2D()
{

}