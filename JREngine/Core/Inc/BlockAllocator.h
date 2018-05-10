#pragma once
#include <vector>

namespace Core
{

class BlockAllocator
{
public:
	BlockAllocator(unsigned int blockSize, unsigned int blockCapacity);
	~BlockAllocator();

	BlockAllocator(const BlockAllocator& copy) = delete;
	BlockAllocator& operator=(const BlockAllocator& copy) = delete;

	void* Allocate();
	void Free(void* ptr);

protected:
	void* mData;
	unsigned int mSize, mCapacity;
	std::vector<unsigned int> mFreeSlots;

}; // class BlockAllocator

} // namespace Core