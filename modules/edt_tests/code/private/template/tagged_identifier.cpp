#include "edt/template/tagged_identifier.hpp"

#include <cstdint>
#include <set>

#include "edt/template/tagged_type_id.hpp"
#include "gtest/gtest.h"

namespace
{
struct TagA;
struct TagB;
using IdA = edt::TaggedIdentifier<TagA, std::uint32_t>;
using IdB = edt::TaggedIdentifier<TagB, std::uint32_t>;
}  // namespace

TEST(TaggedIdentifierTest, DefaultConstructedIsInvalid)
{
    constexpr IdA id;
    static_assert(!id.IsValid());
    static_assert(IdA::FromValue(0).IsValid());
}

TEST(TaggedIdentifierTest, DistinctTagsAreDistinctTypes)
{
    static_assert(!std::is_same_v<IdA, IdB>);
    static_assert(!std::is_convertible_v<IdA, IdB>);
}

// One defaulted == and one defaulted <=> replace the five hand-written operators.
TEST(TaggedIdentifierTest, ComparisonsComeFromSpaceship)
{
    constexpr auto a = IdA::FromValue(1);
    constexpr auto b = IdA::FromValue(2);

    static_assert(a == a);
    static_assert(a != b);
    static_assert(a < b);
    static_assert(b > a);
    static_assert(a <= a);
    static_assert(a >= a);

    // Ordered containers only need <, which is now rewritten from <=>.
    const std::set<IdA> ids{b, a};
    EXPECT_EQ(*ids.begin(), a);
}

TEST(TaggedTypeIdTest, GivesEachTypeItsOwnStableId)
{
    struct First;
    struct Second;

    const auto first = edt::GetTypeId<TagA, std::uint32_t, First>();
    const auto second = edt::GetTypeId<TagA, std::uint32_t, Second>();

    EXPECT_NE(first, second);
    EXPECT_EQ(first, (edt::GetTypeId<TagA, std::uint32_t, First>()));
    EXPECT_TRUE(first.IsValid());
}
