#pragma once
#include <vector>

namespace Core
{

class BlockAllocator
{
public:
	BlockAllocator(int blockSize, int blockCapacity);
	~BlockAllocator();

	BlockAllocator(const BlockAllocator& copy) = delete;
	BlockAllocator& operator=(const BlockAllocator& copy) = delete;

	void* Allocate();
	void Free(void* ptr);

protected:
	void* mData;
	int mSize, mCapacity;
	std::vector<int> mFreeSlots;

}; // class BlockAllocator

} // namespace Core