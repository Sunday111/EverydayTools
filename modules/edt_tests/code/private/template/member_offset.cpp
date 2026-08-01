#include "edt/template/member_offset.hpp"

#include <cstddef>
#include <cstdint>

#include "gtest/gtest.h"

namespace
{
struct Fields
{
    uint32_t first;
    uint8_t second;
    double third;
};

struct Base
{
    uint64_t base_field;
};

struct Derived : Base
{
    uint32_t derived_field;
};
}  // namespace

TEST(MemberOffsetTest, MatchesOffsetof)
{
    EXPECT_EQ(edt::MemberOffset<&Fields::first>(), offsetof(Fields, first));
    EXPECT_EQ(edt::MemberOffset<&Fields::second>(), offsetof(Fields, second));
    EXPECT_EQ(edt::MemberOffset<&Fields::third>(), offsetof(Fields, third));
}

TEST(MemberOffsetTest, FirstMemberIsAtZero)
{
    EXPECT_EQ(edt::MemberOffset<&Fields::first>(), 0u);
}

TEST(MemberOffsetTest, ReachesInheritedMembers)
{
    // Derived is not standard-layout, so offsetof is not available to compare against.
    const Derived value{};
    const auto* object = reinterpret_cast<const char*>(&value);  // NOLINT

    EXPECT_EQ(
        edt::MemberOffset<&Derived::base_field>(),
        static_cast<size_t>(reinterpret_cast<const char*>(&value.base_field) - object));  // NOLINT
    EXPECT_EQ(
        edt::MemberOffset<&Derived::derived_field>(),
        static_cast<size_t>(reinterpret_cast<const char*>(&value.derived_field) - object));  // NOLINT
}
