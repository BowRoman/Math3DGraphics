#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreTest
{
enum class FailEnum
{
	zero = 0x01,
	one = 0x02,
	two = 0x04,
	three = 0x08
};
enum class PassEnum
{
	zero = 0x01,
	one = 0x02,
	two = 0x04,
	three = 0x08,
	enable_bit_flags
};

TEST_CLASS(UnitTest1)
{
public:

	TEST_METHOD(TestOr)
	{
		PassEnum pass = PassEnum::zero | PassEnum::one;
		Assert::AreEqual((int)pass, (0x01 | 0x02));

		pass |= PassEnum::two;
		Assert::AreEqual((int)pass, (0x01 | 0x02 | 0x04));
		FailEnum fail = FailEnum::zero | FailEnum::one;
	}

	TEST_METHOD(TestAnd)
	{
		PassEnum pass = PassEnum::zero & PassEnum::one;
		Assert::AreEqual((int)pass, (0x01 & 0x02));

		pass &= PassEnum::two;
		Assert::AreEqual((int)pass, (0x01 & 0x02 & 0x04));
	}

	TEST_METHOD(TestXOr)
	{
		PassEnum pass = PassEnum::zero ^ PassEnum::one;
		Assert::AreEqual((int)pass, (0x01 ^ 0x02));

		pass ^= PassEnum::two;
		Assert::AreEqual((int)pass, (0x01 ^ 0x02 ^ 0x04));
		//FailEnum fail = FailEnum::zero | FailEnum::one;
	}

	TEST_METHOD(TestNot)
	{
		PassEnum pass = ~PassEnum::zero;
		Assert::AreEqual((int)pass, ~0x01);
	}
};

}