#pragma once

#include "GameObjectFactory.h"

namespace GameEngine
{

class World
{
	using GameObjectVector = std::vector<GameObject*>;

	std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
	std::unique_ptr<GameObjectFactory> mGameObjectFactory;
	std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;

	GameObjectVector mUpdateList;
	GameObjectVector mDestroyList;
	bool bUpdating = false;

public:
	using Visitor = std::function<void(GameObject*)>;
	World();
	~World();

	void Initialize(uint32_t capacity);
	void Terminate();

	void LoadLevel(const char* levelFileName);

	GameObjectHandle Create(const char* templateFileName, const char* name);
	GameObjectHandle Find(const char* name);
	void Destroy(GameObjectHandle gameObj);

	void Visit(Visitor& visitor);

	void Update(float deltaTime);
	void Render();
	void Render2D();

private:
	void DestroyInternal(GameObject* gameObj);
	void PruneDestroyed();

}; // class World

} // namespace GameEngine