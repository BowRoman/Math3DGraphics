#pragma once

class Component;
class GameObject;

using GameObjectAllocator = Core::TypedAllocator<GameObject>;

class GameObject
{
	using Components = std::vector<std::unique_ptr<Component>>;

	Components mComponents;

public:
	GameObject();
	~GameObject();

	void Initialize();
	void Terminate();

	void Update(float dTime);
	void Render();
	void Render2D();

	template <class T>
	typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
		AddComponent();

	template <class T>
	T* GetComponent();

	template <class T>
	const T* GetComponent() const;

}; // class GameObject

template <class T>
typename std::enable_if<std::is_base_of<Component,T>::value, T*>::type
GameObject::AddComponent()
{
	// create a component of the given type and return a pointer to it to be modified
	mComponents.emplace_back(std::make_unique<T>());
	auto& newComp = mComponents.back();
	newComp->mGameObject = this;
	return static_cast<T*>(newComp.get());
}

template <class T>
T* GameObject::GetComponent()
{
	// Function can only be called by someone that has access to the GO,
	// therefore we can safely use the const Get and const cast it to return
	return const_cast<T*>(static_cast<const GameObject*>(this)->GetComponent<T>());
}

template <class T>
const T* GameObject::GetComponent() const
{
	// search for a component of the given type
	for (auto& component : mComponents)
	{
		if (component->GetType() == T::StaticGetType())
		{
			return static_cast<const T*>(component.get());
		}
	}
	// if no such component exists, return nullptr
	return nullptr;
}