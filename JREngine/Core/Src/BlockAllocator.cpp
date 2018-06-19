#include "Precompiled.h"
#include "BlockAllocator.h"
#include "Debug.h"

namespace Core
{

BlockAllocator::BlockAllocator(unsigned int blockSize, unsigned int blockCapacity)
	: mSize{ blockSize }
	, mCapacity{ blockCapacity }
{
	ASSERT(blockSize > 0 && blockCapacity > 0, "[BlockAllocator] Invalid construction parameters.");
	// fill free slot indices
	mFreeSlots.reserve(blockCapacity);
	for (unsigned int i = 0; i < blockCapacity; ++i)
	{
		mFreeSlots.push_back(i);
	}
	// allocate full requested capacity
	mData = malloc(blockSize*blockCapacity);

} // BlockAllocator(int blockSize, int blockCapacity)

BlockAllocator::~BlockAllocator()
{
	free(mData);

} // ~BlockAllocator()

void* BlockAllocator::Allocate()
{
	if (mFreeSlots.size() <= 0)
	{
		return nullptr;
	}

	// remove slot from freeslots
	int offset = mFreeSlots.back();
	mFreeSlots.pop_back();

	// adjust by block size
	offset *= mSize;

	// move ptr to that point
	void* ptr = static_cast<void*>(static_cast<uint8_t*>(mData) + static_cast<uint8_t>(offset));

	return ptr;

} // void* Allocate()

void BlockAllocator::Free(void* ptr)
{
	// TODO: check if ptr is contained in allocator
	// add pointer block index back into freeslots
	mFreeSlots.emplace_back(reinterpret_cast<int>(ptr) - reinterpret_cast<int>(mData));

} // void Free(void* ptr)

} // namespace Core
