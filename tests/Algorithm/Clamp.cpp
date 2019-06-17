#include "gtest/gtest.h"
#include "EverydayTools/Algorithm/Clamp.h"
#include "EverydayTools/UnusedVar.h"

TEST(AlgorithmTest, Clamp)
{
	using namespace edt;

	// Just usual case
	EXPECT_TRUE(Clamp(10, 9, 12) == 10);
	EXPECT_TRUE(Clamp(10, 10, 12) == 10);
	EXPECT_TRUE(Clamp(10, 11, 12) == 11);
	EXPECT_TRUE(Clamp(10, 12, 12) == 12);
	EXPECT_TRUE(Clamp(10, 13, 12) == 12);

	// Check that it works with negative numbers
	EXPECT_TRUE(Clamp(-12, -9, -10) == -10);
	EXPECT_TRUE(Clamp(-12, -10, -10) == -10);
	EXPECT_TRUE(Clamp(-12, -11, -10) == -11);
	EXPECT_TRUE(Clamp(-12, -12, -10) == -12);
	EXPECT_TRUE(Clamp(-12, -13, -10) == -12);
}

TEST(MathTest, ClampToAnotherTypeBounds)
{
	using namespace edt;

	// Just usual cases
	EXPECT_TRUE(ClampToAnotherTypeBounds<int8_t>(int16_t(10)) == 10);
	EXPECT_TRUE(ClampToAnotherTypeBounds<int8_t>(int16_t(130)) == 127);
	EXPECT_TRUE(ClampToAnotherTypeBounds<int8_t>(int16_t(-130)) == -128);

	// signed to unsigned
	EXPECT_TRUE(ClampToAnotherTypeBounds<uint8_t>(int16_t(10)) == 10);
	EXPECT_TRUE(ClampToAnotherTypeBounds<uint8_t>(int16_t(256)) == 255);
	EXPECT_TRUE(ClampToAnotherTypeBounds<uint8_t>(int16_t(-10)) == 0);

	// Unsigned to signed
	EXPECT_TRUE(ClampToAnotherTypeBounds<int8_t>(uint16_t(10)) == 10);
	EXPECT_TRUE(ClampToAnotherTypeBounds<int8_t>(uint16_t(128)) == 127);
}
