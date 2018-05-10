#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	mComponents.clear();
}

void GameObject::Initialize()
{
	for (auto& component : mComponents)
	{
		component->Initialize();
	}
}

void GameObject::Terminate()
{
	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void GameObject::Update(float dTime)
{
	for (auto& component : mComponents)
	{
		component->Update(dTime);
	}
}

void GameObject::Render()
{
	for (auto& component : mComponents)
	{
		component->Render();
	}
}

void GameObject::Render2D()
{
	for (auto& component : mComponents)
	{
		component->Render2D();
	}
}
