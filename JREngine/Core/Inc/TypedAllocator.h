#pragma once
#include "BlockAllocator.h"

namespace Core
{

template<typename T>
class TypedAllocator : private BlockAllocator
{
public:
	TypedAllocator(unsigned int blockCapacity);
	~TypedAllocator();

	TypedAllocator(const TypedAllocator<T>& copy) = delete;
	TypedAllocator& operator=(const TypedAllocator<T>& copy) = delete;

	T* New();
	void Delete(T* ptr);

}; // class TypedAllocator : private BlockAllocator

template<typename T>
TypedAllocator<T>::TypedAllocator(unsigned int blockCapacity)
	: BlockAllocator(sizeof(T), blockCapacity)
{
} // TypedAllocator(int blockCapacity)

template<typename T>
TypedAllocator<T>::~TypedAllocator()
{
} // ~TypedAllocator()

template<typename T>
T* TypedAllocator<T>::New()
{
	T* ptr = static_cast<T*>(Allocate());
	// if the pointer was successfully allocated, initialize the object
	if (ptr != nullptr)
	{
		new(ptr) T();
	}
	return ptr;

} // T* New()

template<typename T>
void TypedAllocator<T>::Delete(T* ptr)
{
	//TODO: Check if pointer belongs to allocator
	// destruct the object
	ptr->~T();
	Free(static_cast<void*>(ptr));

} // void Delete(T* ptr)

} // namespace Core