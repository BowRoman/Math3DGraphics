#pragma once
#include <vector>

class BlockAllocator
{
public:
	BlockAllocator(int blockSize, int blockCount);
	~BlockAllocator();

	BlockAllocator(const BlockAllocator& copy) = delete;
	BlockAllocator& operator=(const BlockAllocator& copy) = delete;

	void* Allocate();
	void Free(void* ptr);
protected:
	void* mData;
	int mSize, mCapacity;
	std::vector<int> mFreeSlots;
};