#include "Precompiled.h"
#include "World.h"

#include "AABoxColliderComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

void World::Initialize(uint32_t capacity)
{
	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);
	mUpdateList.reserve(capacity);
	mDestroyList.reserve(capacity);
	mGameObjectFactory->Register("CameraComponent", CameraComponent::CreateFunc);
	mGameObjectFactory->Register("AABoxColliderComponent", AABoxColliderComponent::CreateFunc);
	mGameObjectFactory->Register("TransformComponent", TransformComponent::CreateFunc);
}

void World::Terminate()
{
	ASSERT(!bUpdating, "[World] Cannot be terminating during update.");

	mDestroyList.insert(std::end(mDestroyList), std::begin(mUpdateList), std::end(mUpdateList));
	
	mUpdateList.clear();
	PruneDestroyed();

	mGameObjectAllocator.reset();
	mGameObjectFactory.reset();

}

void World::LoadLevel(const char* levelFileName)
{
	TiXmlDocument doc(levelFileName);
	ASSERT(doc.LoadFile(), "[World] Failed to open level file.");

	TiXmlHandle handleDoc(&doc);

	// root
	TiXmlElement* element = handleDoc.FirstChildElement().Element();
	if (element)
	{
		TiXmlHandle handleRoot = TiXmlHandle(element);

		// load all files
		element = handleRoot.FirstChild("GameObject").FirstChild().Element();
		while (element)
		{
			TiXmlNode* name = element->FirstChild();
			TiXmlNode* fileNode = name->NextSibling();
			auto obj = Create(fileNode->Value(), name->Value());

			const TiXmlNode* positionNode = fileNode->NextSibling();
			if (positionNode)
			{
				auto transformComp = obj->GetComponent<TransformComponent>();
				if (transformComp)
				{
					auto xNode = positionNode->FirstChild()->FirstChild();
					float x = static_cast<float>(std::atof(positionNode->Value()));

					auto yNode = xNode->NextSiblingElement()->FirstChild();
					float y = static_cast<float>(std::atof(yNode->Value()));

					auto zNode = yNode->NextSiblingElement()->FirstChild();
					float z = static_cast<float>(std::atof(zNode->Value()));
					transformComp->SetPosition({ x,y,z });
				}
			}
			// TODO: Override data for each component

			element->NextSiblingElement();
		}
	}
}

GameObject* World::Create(const char* templateFileName, const char* name)
{
	// TODO: Integrate name into creation and find
	GameObject* object = mGameObjectFactory->Create(templateFileName);
	ASSERT(object, "[World] Failed to create GameObject.");

	object->mWorld = this;
	object->mName = std::string(name);
	object->Initialize();

	mUpdateList.push_back(object);

	return object;
}

GameObject* World::Find(const char* name)
{
	for (auto obj : mUpdateList)
	{
		if (obj->GetName() == std::string(name))
		{
			return obj;
		}
	}
	return nullptr;
}

void World::Destroy(GameObject* gameObj)
{
	if (gameObj == nullptr)
	{
		return;
	}
	if (!bUpdating)
	{
		DestroyInternal(gameObj);
	}
	else
	{
		mDestroyList.push_back(gameObj);
	}
}

void World::Update(float deltaTime)
{
	ASSERT(!bUpdating, "[World] Update already in progress.");

	bUpdating = true;

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObj = mUpdateList[i];
		gameObj->Update(deltaTime);
	}

	bUpdating = false;

	PruneDestroyed();
}

void World::Render()
{
	for (auto obj : mUpdateList)
	{
		obj->Render();
	}
}

void World::Render2D()
{
	for (auto obj : mUpdateList)
	{
		obj->Render2D();
	}
}

void World::DestroyInternal(GameObject* gameObj)
{
	ASSERT(!bUpdating, "[World] Object cannot be destroyed during update.");

	if (gameObj == nullptr)
	{
		return;
	}

	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObj);
	if (iter != mUpdateList.end())
	{
		mUpdateList.erase(iter);
	}

	gameObj->Terminate();

	mGameObjectFactory->Destroy(gameObj);
}

void World::PruneDestroyed()
{
	for (auto obj : mDestroyList)
	{
		DestroyInternal(obj);
	}
	mDestroyList.clear();
}
