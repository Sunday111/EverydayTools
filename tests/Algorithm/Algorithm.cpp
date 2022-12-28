#include <memory>

#include "EverydayTools/Algorithm/Compare.hpp"
#include "EverydayTools/UnusedVar.hpp"
#include "gtest/gtest.h"

TEST(AlgorithmTest, MinInteger) {
  using namespace edt;

  using namespace detail;

  {
    static_assert(MinInteger(11, 10) == 10);
    static_assert(MinInteger(10, 11, 12) == 10);
  }

  {
    int a = 10, b = 20, c = 30;
    MinInteger(a, b, c) *= 2;
    EXPECT_TRUE(a == 20);
  }

  {
    auto k1 = std::make_unique<int>();
    auto k2 = std::make_unique<int>();
    auto k3 = std::make_unique<int>();

    auto l = MinInteger(std::move(k1), std::move(k2), std::move(k3));
    EXPECT_TRUE(k1 == nullptr || k2 == nullptr || k3 == nullptr);
  }
}