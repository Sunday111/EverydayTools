#include "EverydayTools/Template/StaticFor.hpp"

#include <array>
#include <cstddef>
#include <vector>

#include <gtest/gtest.h>

namespace {

consteval auto MakeIndices() {
  std::array<std::size_t, 4> result{};
  edt::StaticFor<4>([&]<std::size_t index> { result[index] = index; });
  return result;
}

static_assert(MakeIndices() == std::array<std::size_t, 4>{0, 1, 2, 3});

struct NonMovableVisitor {
  explicit NonMovableVisitor(std::vector<std::size_t> &in_visited)
      : visited(in_visited) {}

  NonMovableVisitor(const NonMovableVisitor &) = delete;
  NonMovableVisitor(NonMovableVisitor &&) = delete;

  template <std::size_t index> void operator()() { visited.push_back(index); }

  std::vector<std::size_t> &visited;
};

TEST(StaticForTest, VisitsIndicesInOrder) {
  std::vector<std::size_t> visited;
  edt::StaticFor<4>([&]<std::size_t index> { visited.push_back(index); });

  EXPECT_EQ(visited, (std::vector<std::size_t>{0, 1, 2, 3}));
}

TEST(StaticForTest, VisitsSubrangeInOrder) {
  std::vector<std::size_t> visited;
  edt::StaticFor<3, 7>([&]<std::size_t index> { visited.push_back(index); });

  EXPECT_EQ(visited, (std::vector<std::size_t>{3, 4, 5, 6}));
}

TEST(StaticForTest, SupportsEmptyRanges) {
  bool invoked = false;
  edt::StaticFor<0>([&]<std::size_t> { invoked = true; });
  edt::StaticFor<5, 5>([&]<std::size_t> { invoked = true; });

  EXPECT_FALSE(invoked);
}

TEST(StaticForTest, DoesNotMoveCallableBetweenIterations) {
  std::vector<std::size_t> visited;
  edt::StaticFor<3>(NonMovableVisitor{visited});

  EXPECT_EQ(visited, (std::vector<std::size_t>{0, 1, 2}));
}

} // namespace
