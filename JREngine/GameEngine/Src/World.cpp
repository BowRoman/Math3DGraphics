#include "Precompiled.h"
#include "World.h"

#include "AABoxColliderComponent.h"
#include "CameraComponent.h"
#include "FPControllerComponent.h"
#include "TransformComponent.h"

namespace GameEngine
{

World::World()
{
}

World::~World()
{
}

void World::Initialize(uint32_t capacity)
{
	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);

	mUpdateList.reserve(capacity);
	mDestroyList.reserve(capacity);

	mGameObjectFactory->Register("CameraComponent", CameraComponent::CreateFunc);
	mGameObjectFactory->Register("ColliderComponent", AABoxColliderComponent::CreateFunc);
	mGameObjectFactory->Register("FPControllerComponent",FPControllerComponent::CreateFunc);
	mGameObjectFactory->Register("TransformComponent", TransformComponent::CreateFunc);
}

void World::Terminate()
{
	ASSERT(!bUpdating, "[World] Cannot be terminating during update.");

	bUpdating = true;
	for (auto obj : mUpdateList)
	{
		Destroy(obj->GetHandle());
	}
	bUpdating = false;
	
	PruneDestroyed();
	mUpdateList.clear();

	mGameObjectAllocator.reset();
	mGameObjectFactory.reset();
	mGameObjectHandlePool.reset();
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
		element = handleRoot.FirstChild("GameObject").Element();
		while (element)
		{
			TiXmlNode* name = element->FirstChild();
			TiXmlNode* fileNode = name->NextSibling();
			auto obj = Create(fileNode->FirstChild()->Value(), name->FirstChild()->Value());

			const auto* overrideNode = fileNode->NextSibling();
			if (overrideNode)
			{
				if (std::strcmp(overrideNode->FirstChildElement()->FirstAttribute()->Value(), "Position") == 0)
				{
					auto transformComp = obj->GetComponent<TransformComponent>();
					if (transformComp)
					{
						auto xNode = overrideNode->FirstChild();
						float x = static_cast<float>(std::atof(xNode->FirstChild()->Value()));

						auto yNode = xNode->NextSiblingElement();
						float y = static_cast<float>(std::atof(yNode->FirstChild()->Value()));

						auto zNode = yNode->NextSiblingElement();
						float z = static_cast<float>(std::atof(zNode->FirstChild()->Value()));
						transformComp->SetPosition({ x,y,z });
					}
				}
			}
			// TODO: Override data for each component

			element = element->NextSiblingElement();
		}
	}
}

GameObjectHandle World::Create(const char* templateFileName, const char* name)
{
	GameObject* object = mGameObjectFactory->Create(templateFileName);
	ASSERT(object, "[World] Failed to create GameObject.");

	auto handle = mGameObjectHandlePool->Register(object);

	object->mWorld = this;
	object->mName = std::string(name);
	object->mHandle = handle;
	object->Initialize();

	mUpdateList.push_back(object);

	return handle;
}

GameObjectHandle World::Find(const char* name)
{
	for (auto obj : mUpdateList)
	{
		if (obj->GetName() == std::string(name))
		{
			return obj->GetHandle();
		}
	}
	return GameObjectHandle();
}

void World::Destroy(GameObjectHandle handle)
{
	if (!handle.IsValid())
	{
		return;
	}

	GameObject* obj = handle.Get();

	// mark object and free handle
	mGameObjectHandlePool->Unregister(handle);

	if (!bUpdating)
	{
		DestroyInternal(obj);
	}
	else
	{
		mDestroyList.push_back(obj);
	}
}

void World::Visit(Visitor & visitor)
{
	for (auto gameObj : mUpdateList)
	{
		visitor(gameObj);
	}
}

void World::Update(float deltaTime)
{
	ASSERT(!bUpdating, "[World] Update already in progress.");

	bUpdating = true;

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObj = mUpdateList[i];
		// object may have been deleted
		if (gameObj->GetHandle().IsValid())
		{
			gameObj->Update(deltaTime);
		}
	}

	bUpdating = false;

	PruneDestroyed();
}

void World::Render()
{
	Graphics::GraphicsSystem::Get()->BeginRender();

	auto camera = Find("FPSCamera").Get();
	if (camera)
	{
		auto rawcamera = camera->GetComponent<GameEngine::CameraComponent>()->GetCamera();
		auto cameraTrans = rawcamera.mTransform;

		Math::Matrix4 viewMatrix = rawcamera.GetViewMatrix(cameraTrans);
		Math::Matrix4 projectionMatrix = rawcamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

		for (auto obj : mUpdateList)
		{
			obj->Render();
		}

		for (int i = 0; i < 100; ++i)
		{
			Math::Vector3 p0(-50.0f, -0.1f, -50.0f + i);
			Math::Vector3 p1(+50.0f, -0.1f, -50.0f + i);
			Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Gray());
		}
		for (int i = 0; i < 100; ++i)
		{
			Math::Vector3 p0(-50.0f + i, -0.1f, -50.0f);
			Math::Vector3 p1(-50.0f + i, -0.1f, +50.0f);
			Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Gray());
		}

		Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);
	}

	Graphics::GraphicsSystem::Get()->EndRender();

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

} // namespace GameEngine
