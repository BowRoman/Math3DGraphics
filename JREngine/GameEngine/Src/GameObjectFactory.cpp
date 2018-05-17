#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "AABoxColliderComponent.h"
#include "TransformComponent.h"

GameObjectFactory::GameObjectFactory(GameObjectAllocator& allocator)
	: mGameObjectAllocator(allocator)
{
}

bool GameObjectFactory::Register(std::string name, CreateFunc func)
{
	auto result = mCreateFuncMap.emplace(std::move(name), func);
	return result.second;
}

GameObject* GameObjectFactory::Create(const char* templateFileName)
{
	GameObject* gameObject = mGameObjectAllocator.New();

	TiXmlDocument doc(templateFileName);
	if (!doc.LoadFile())
	{
		return nullptr;
	}

	TiXmlHandle handleDoc(&doc);
	TiXmlElement* element;
	TiXmlHandle handleRoot(0);

	// root
	element = handleDoc.FirstChildElement().Element();
	if (!element)
	{
		return nullptr;
	}
	handleRoot = TiXmlHandle(element);

	// check all components
	element = handleRoot.FirstChild("Components").FirstChild().Element();
	for (;; element = element->NextSiblingElement())
	{
		const char *ComponentType = element->Value();

		mCreateFuncMap[std::string(ComponentType)](gameObject, element);
	}

	return gameObject;
}

void GameObjectFactory::Destroy(GameObject* gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}