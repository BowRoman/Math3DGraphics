#pragma once
#include "GameObject.h"

#include <functional>

class GameObjectFactory
{
	using CreateFunc = std::function<void(GameObject*, const TiXmlNode*)>;

	GameObjectAllocator& mGameObjectAllocator;
	std::unordered_map<std::string, CreateFunc> mCreateFuncMap;

public:

	GameObjectFactory(GameObjectAllocator& allocator);

	bool Register(std::string name, CreateFunc func);

	GameObject* Create(const char* templateFileName);
	void Destroy(GameObject* gameObject);
};