#pragma once

#include "Common.h"

namespace Core
{

template<class DataType>
class HandlePool;

template<class DataType>
class Handle
{
	friend class  HandlePool<DataType>;
	static HandlePool<DataType>* sPool;
	uint32_t mIndex : 24;
	uint32_t mGeneration : 8;

public:
	Handle();

	bool IsValid() const;
	void Invalidate();

	DataType* Get() const;
	DataType* operator->() const;

	bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
	bool operator!=(Handle rhs) const { return !(*this == rhs); }
};

template<class DataType>
class HandlePool
{
	struct Entry
	{
		DataType* instance = nullptr;
		uint32_t generation = 0;
	};

	std::vector<Entry> mEntries;
	std::vector<uint32_t> mFreeSlots;

public:
	using HandleType = Handle<DataType>;

	HandlePool(uint32_t capacity);
	~HandlePool();

	HandleType Register(DataType* instance);
	void Unregister(HandleType handle);

	bool IsValid(HandleType handle) const;
	DataType* Get(HandleType handle);

};

template<class DataType>
HandlePool<DataType>* Handle<DataType>::sPool = nullptr;

template<class DataType>
Handle<DataType>::Handle()
	: mIndex{ 0 }
	, mGeneration{ 0 }
{
}

template<class DataType>
bool Handle<DataType>::IsValid() const
{
	return sPool && sPool->IsValid(*this);
}

template<class DataType>
void Handle<DataType>::Invalidate()
{
	*this = Handle();
}

template<class DataType>
DataType* Handle<DataType>::Get() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}

template<class DataType>
DataType* Handle<DataType>::operator->() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}

template<class DataType>
HandlePool<DataType>::HandlePool(uint32_t capacity)
{
	ASSERT(capacity > 0, "[HandlePool] Invalid capacity.");

	mEntries.resize(capacity + 1);
	mFreeSlots.reserve(capacity + 1);

	for (size_t i = capacity + 1; i > 0; --i)
	{
		mFreeSlots.push_back(i);
	}

	ASSERT(HandleType::sPool == nullptr, "[HandlePool] Cannot create multiple pools of the same type.");

	HandleType::sPool = this;
}

template<class DataType>
HandlePool<DataType>::~HandlePool()
{
	ASSERT(mFreeSlots.size() >= mFreeSlots.capacity(), "[HandlePool] Pool cannot be destructed with registered slots.");

	ASSERT(HandleType::sPool == this, "[HandlePool] Pool cannot be destructed, something went wrong.");
	HandleType::sPool = nullptr;
}

template<class DataType>
Handle<DataType> HandlePool<DataType>::Register(DataType* instance)
{
	ASSERT(instance != nullptr, "[HandlePool] Invalid instance.");
	ASSERT(!mFreeSlots.empty(), "[HandlePool] Pool is full.");

	// find free slot
	uint32_t slot = mFreeSlots.back();
	mFreeSlots.pop_back();

	// register instance
	mEntries[slot].instance = instance;

	// create handle to entry
	HandleType handle;
	handle.mIndex = slot;
	handle.mGeneration = mEntries[slot].generation;

	return handle;
}

template<class DataType>
void HandlePool<DataType>::Unregister(HandleType handle)
{
	if (IsValid(handle))
	{
		mEntries[handle.mIndex].generation++;
		mFreeSlots.push_back(handle.mIndex);
	}
}

template<class DataType>
bool HandlePool<DataType>::IsValid(HandleType handle) const
{
	return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
}

template<class DataType>
DataType* HandlePool<DataType>::Get(HandleType handle)
{
	return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
}


} // namespace Core