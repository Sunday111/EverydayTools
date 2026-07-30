#include <format>
#include <sstream>
#include <unordered_set>

#include "edt/guid.hpp"
#include "gtest/gtest.h"

namespace
{
constexpr std::string_view kText = "01234567-89AB-CDEF-0123-456789ABCDEF";
}

TEST(GuidTest, ThreeWayComparison)
{
    constexpr auto a = edt::GUID::Create("00000000-0000-0000-0000-000000000001");
    constexpr auto b = edt::GUID::Create("00000000-0000-0000-0000-000000000002");

    static_assert(a == a);
    static_assert(a != b);
    static_assert(a < b);
    static_assert(b > a);
    static_assert(a <= a);
    static_assert(b >= a);
}

TEST(GuidTest, StdFormat)
{
    constexpr auto guid = edt::GUID::Create(kText);
    EXPECT_EQ(std::format("{}", guid), kText);
}

TEST(GuidTest, StdFormatHonoursWidthAndAlignment)
{
    constexpr auto guid = edt::GUID::Create(kText);
    EXPECT_EQ(std::format("[{:>38}]", guid), std::format("[  {}]", kText));
}

TEST(GuidTest, OstreamInsertionIsFoundByAdl)
{
    constexpr auto guid = edt::GUID::Create(kText);
    std::ostringstream out;
    out << guid;
    EXPECT_EQ(out.str(), kText);
}

TEST(GuidTest, IsHashable)
{
    std::unordered_set<edt::GUID> set;
    set.insert(edt::GUID::Create(kText));
    set.insert(edt::GUID::Create(kText));
    EXPECT_EQ(set.size(), 1U);

    // The two halves must not be interchangeable, as a plain xor would make them.
    const edt::GUID a{.part1 = 1, .part2 = 2};
    const edt::GUID b{.part1 = 2, .part2 = 1};
    EXPECT_NE(std::hash<edt::GUID>{}(a), std::hash<edt::GUID>{}(b));
}
