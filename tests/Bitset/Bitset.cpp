#include "EverydayTools/Bitset/Bitset.hpp"

#include "gtest/gtest.h"

using namespace edt;

TEST(BitsetTest, construction) { [[maybe_unused]] Bitset a(10); }

TEST(BitsetTest, SetGet) {
  Bitset a(10);

  a.Set(0);
  EXPECT_TRUE(a.IsSet(0));

  a.Set(1, false);
  EXPECT_FALSE(a.IsSet(1));
}

TEST(BitsetTest, All) {
  Bitset a(2);
  EXPECT_FALSE(a.All());
  a.Set(0);
  EXPECT_FALSE(a.All());
  a.Set(1);
  EXPECT_TRUE(a.All());
}
