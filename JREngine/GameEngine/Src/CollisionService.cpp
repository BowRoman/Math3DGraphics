#include "Precompiled.h"
#include "CollisionService.h"

#include "AABoxColliderComponent.h"

namespace GameEngine
{

CollisionService::CollisionService()
{
}

CollisionService::~CollisionService()
{
}

void CollisionService::Initialize()
{
}

void CollisionService::Terminate()
{
}

void CollisionService::Update(float dTime)
{
	for (auto comp1 : mSubscribers)
	{
		for (auto comp2 : mSubscribers)
		{
			if (comp1 == comp2)
			{
				continue;
			}
			if (CheckCollision(comp1, comp2))
			{
				comp1->OnCollision();
				comp2->OnCollision();
			}
		}
	}
}

void CollisionService::Register(AABoxColliderComponent* component)
{
	mSubscribers.push_back(component);
}

void CollisionService::Unregister(AABoxColliderComponent* component)
{
	auto findIter = std::find(mSubscribers.begin, mSubscribers.end, component);
	if (findIter != mSubscribers.end)
	{
		mSubscribers.erase(findIter);
	}
}

}