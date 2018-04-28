#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestAllocate)
		{
			BlockAllocator allocator(4, 1);
			void* block = allocator.Allocate();
			Assert::IsNotNull(block);
		}
		TEST_METHOD(TestAllocateTwice)
		{
			BlockAllocator allocator(4, 1);
			void* block = allocator.Allocate();
			Assert::IsNotNull(block);

			void* block2 = allocator.Allocate();
			Assert::IsNull(block2);
		}
		TEST_METHOD(TestAllocateTwiceAgain)
		{
			BlockAllocator allocator(4, 2);

			void* block = allocator.Allocate();
			Assert::IsNotNull(block);

			void* block2 = allocator.Allocate();
			Assert::IsNotNull(block2);
		}
		TEST_METHOD(TestReallocate)
		{
			BlockAllocator allocator(4, 1);

			void* block = allocator.Allocate();
			Assert::IsNotNull(block);

			allocator.Free(block);

			void* block2 = allocator.Allocate();
			Assert::IsNotNull(block2);

			Assert::IsTrue(block == block2);
		}
	};
}