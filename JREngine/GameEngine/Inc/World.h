#pragma once

#include "GameObjectFactory.h"

class World
{
	using GameObjectVector = std::vector<GameObject*>;

	std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
	std::unique_ptr<GameObjectFactory> mGameObjectFactory;
	GameObjectVector mGameObjectVector;

public:
	World();
	~World();

	void Initialize(uint32_t capacity);
	void Terminate();

	void LoadLevel(const char* levelFileName);

	GameObject* Create(const char* templateFileName, const char* name);
	GameObject* Find(const char* name);
	void Destroy(GameObject* gameObj);

	void Update(float deltaTime);
	void Render();
	void Render2D();

}; // class World