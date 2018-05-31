#pragma once

#include "GameObjectFactory.h"
#include "Service.h"

namespace GameEngine
{

class World
{
	using GameObjectVector = std::vector<GameObject*>;
	using ServiceVector = std::vector<std::unique_ptr<Service>>;

	std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
	std::unique_ptr<GameObjectFactory> mGameObjectFactory;
	std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;

	GameObjectVector mUpdateList;
	GameObjectVector mDestroyList;
	ServiceVector mServices;
	bool bUpdating = false;

public:
	using Visitor = std::function<void(GameObject*)>;
	using OnRegisterComponent = std::function<void()>;

	World();
	~World();

	void Initialize(uint32_t capacity, OnRegisterComponent registerComponentCB = []() {});
	void Terminate();

	void LoadLevel(const char* levelFileName);

	GameObjectHandle Create(const char* templateFileName, const char* name);
	GameObjectHandle Find(const char* name);
	void Destroy(GameObjectHandle gameObj);

	void Visit(Visitor& visitor);

	void Update(float deltaTime);
	void Render();
	void Render2D();

	template <class T>
	typename std::enable_if<std::is_base_of<Service, T>::value, T*>::type
		AddService();

	template <class T>
	T* GetService();
	template <class T>
	const T* GetService() const;

private:
	void DestroyInternal(GameObject* gameObj);
	void PruneDestroyed();

}; // class World

template <class T>
typename std::enable_if<std::is_base_of<Service, T>::value, T*>::type
World::AddService()
{
	// create a service of the given type and return a pointer to it to be modified
	mServices.emplace_back(std::make_unique<T>());
	auto& newServ = mServices.back();
	newServ->mWorld = this;
	return static_cast<T*>(newComp.get());
}

template <class T>
T* World::GetService()
{
	// use const Get and const cast it to return
	return const_cast<T*>(static_cast<const World*>(this)->GetComponent<T>());
}

template <class T>
const T* World::GetService() const
{
	// search for a service of the given type
	for (auto& service : mServices)
	{
		if (service->GetType() == T::StaticGetType())
		{
			return static_cast<const T*>(service.get());
		}
	}
	// if no such service exists, return nullptr
	return nullptr;
}

} // namespace GameEngine