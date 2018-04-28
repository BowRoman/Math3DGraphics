#pragma once
#include "BlockAllocator.h"

template<typename T>
class TypedAllocator: private BlockAllocator
{
public:
	TypedAllocator(int blockCapacity);
	~TypedAllocator();

	TypedAllocator(const TypedAllocator<T>& copy) = delete;
	TypedAllocator& operator=(const TypedAllocator<T>& copy) = delete;

	T* New();
	void Delete(T* ptr);
};

template<typename T>
TypedAllocator<T>::TypedAllocator(int blockCapacity)
	: BlockAllocator(sizeof(T), blockCapacity)
{
}

template<typename T>
TypedAllocator<T>::~TypedAllocator()
{

}

template<typename T>
T* TypedAllocator<T>::New()
{
	T* ptr = static_cast<T*>(Allocate());
	if (ptr != nullptr)
	{
		new(ptr) T();
	}
	return ptr;
}

template<typename T>
void TypedAllocator<T>::Delete(T* ptr)
{
	ptr->~T();
	Free(static_cast<void*>(ptr));
}