#include "Precompiled.h"
#include "BlockAllocator.h"

BlockAllocator::BlockAllocator(int blockSize, int blockCapacity)
{
	mSize = blockSize;
	mCapacity = blockCapacity;
	mFreeSlots.reserve(blockCapacity);
	for (int i = 0; i < blockCapacity; ++i)
	{
		mFreeSlots.push_back(i);
	}
	mData = malloc(mSize*mCapacity);
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
}

void* BlockAllocator::Allocate()
{
	if(mFreeSlots.size() <= 0)
		return nullptr;

	// remove slot from freeslots
	int offset = mFreeSlots.back();
	mFreeSlots.pop_back();

	// adjust by block size
	offset *= mSize;

	// move ptr to that point
	void* ptr = (void*)((uint8_t)mData * (uint8_t)mSize);

	return ptr;
}

void BlockAllocator::Free(void* ptr)
{
	mFreeSlots.emplace_back(int(ptr) - int(mData));
}
