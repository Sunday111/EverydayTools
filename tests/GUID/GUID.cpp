#include "EverydayTools/GUID.hpp"

#include "gtest/gtest.h"

TEST(GUIDTest, Create) {
  using namespace edt;
  constexpr auto guid = GUID::Create("22345200-abe8-4f60-90c8-0d43c5f6c0f6");
  EXPECT_TRUE(0x4F60ABE822345200 == guid.part1);
  EXPECT_TRUE(0xF6C0F6C5430DC890 == guid.part2);
  static constexpr auto ca = guid.ToCharArray();
  constexpr std::string_view sv(ca.data(), ca.size());
  EXPECT_TRUE(sv == "22345200-ABE8-4F60-90C8-0D43C5F6C0F6");
}
