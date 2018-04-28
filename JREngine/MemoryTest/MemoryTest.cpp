#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryTest
{
	TEST_CLASS(UnitTest1)
	{
	public:

		//---------BlockAllocator---------

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

		//---------TypedAllocator---------

		TEST_METHOD(TestNew)
		{
			TypedAllocator<int> allocator(1);
			int* block = allocator.New();
			Assert::IsNotNull(block);
		}

		TEST_METHOD(TestNewTwice)
		{
			TypedAllocator<int> allocator(1);
			int* block = allocator.New();
			Assert::IsNotNull(block);

			int* block2 = allocator.New();
			Assert::IsNull(block2);
		}

		TEST_METHOD(TestNewTwiceAgain)
		{
			TypedAllocator<int> allocator(2);

			int* block = allocator.New();
			Assert::IsNotNull(block);

			int* block2 = allocator.New();
			Assert::IsNotNull(block2);
		}

		TEST_METHOD(TestRenew)
		{
			TypedAllocator<int> allocator(1);

			int* block = allocator.New();
			Assert::IsNotNull(block);

			allocator.Delete(block);

			void* block2 = allocator.New();
			Assert::IsNotNull(block2);

			Assert::IsTrue(block == block2);
		}

		TEST_METHOD(TestRenewAgain)
		{
			TypedAllocator<int> allocator(1);

			int* block = allocator.New();
			Assert::IsNotNull(block);

			void* block2 = allocator.New();
			Assert::IsNull(block2);

			allocator.Delete(block);

			block2 = allocator.New();
			Assert::IsNotNull(block2);

			Assert::IsTrue(block == block2);
		}

		TEST_METHOD(TestCustomType)
		{
			struct Foo
			{
				int i = 3;
				char k = 'k';
			};
			TypedAllocator<Foo> allocator(2);

			Foo* block = allocator.New();
			Assert::IsNotNull(block);
			Assert::IsTrue(block->i == 3);

			Foo* block2 = allocator.New();
			Assert::IsNotNull(block2);
			Assert::AreEqual(block->k, block2->k);
		}
	};
}