#include <cstdint>
#include <limits>
#include <utility>

#include "edt/algorithm/clamp.hpp"
#include "gtest/gtest.h"

namespace
{
// The obvious implementation, written so the two limits are only ever compared
// across types through std::cmp_*. Both casts here are reachable only once the
// comparison has proved the bound fits in From.
template <typename Bounds, typename From>
constexpr From Reference(From value)
{
    if (std::cmp_less(value, std::numeric_limits<Bounds>::lowest()))
    {
        return static_cast<From>(std::numeric_limits<Bounds>::lowest());
    }
    if (std::cmp_greater(value, std::numeric_limits<Bounds>::max()))
    {
        return static_cast<From>(std::numeric_limits<Bounds>::max());
    }
    return value;
}

// Every value of From, for the narrow types - enumerated as bit patterns, so no
// sampling and no chosen constants.
template <typename Bounds, typename From>
void CheckEveryValue()
{
    using Bits = std::make_unsigned_t<From>;
    constexpr std::uint64_t kPatternCount = std::uint64_t{std::numeric_limits<Bits>::max()} + 1;

    for (std::uint64_t pattern = 0; pattern != kPatternCount; ++pattern)
    {
        const auto value = static_cast<From>(static_cast<Bits>(pattern));
        const From actual = edt::ClampToAnotherTypeBounds<Bounds>(value);
        const From expected = Reference<Bounds>(value);
        ASSERT_EQ(actual, expected) << "bit pattern = " << pattern << " (" << sizeof(From) << "-byte source, "
                                    << sizeof(Bounds) << "-byte bounds)";

        // Whatever comes out must survive the trip into Bounds and back.
        ASSERT_TRUE(std::in_range<Bounds>(actual)) << "bit pattern = " << pattern;
    }
}

template <typename Bounds>
void CheckAgainstEveryNarrowSource()
{
    CheckEveryValue<Bounds, std::int8_t>();
    CheckEveryValue<Bounds, std::uint8_t>();
    CheckEveryValue<Bounds, std::int16_t>();
    CheckEveryValue<Bounds, std::uint16_t>();
}
}  // namespace

// The full 4x4 matrix of narrow source and bound types, every value of each source.
TEST(ClampTest, MatchesTheReferenceForEveryNarrowValue)  // NOLINT
{
    CheckAgainstEveryNarrowSource<std::int8_t>();
    CheckAgainstEveryNarrowSource<std::uint8_t>();
    CheckAgainstEveryNarrowSource<std::int16_t>();
    CheckAgainstEveryNarrowSource<std::uint16_t>();
}

// Wide bounds cannot clip a narrow source at all: the result is always the input.
TEST(ClampTest, WiderBoundsAreATransparentPassThrough)  // NOLINT
{
    CheckEveryValue<std::int64_t, std::int8_t>();
    CheckEveryValue<std::int64_t, std::uint8_t>();
    CheckEveryValue<std::uint64_t, std::uint16_t>();

    for (int raw = -128; raw != 128; ++raw)
    {
        const auto value = static_cast<std::int8_t>(raw);
        EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int64_t>(value), value);
        EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int32_t>(value), value);
    }
}

// Clamping to the source's own type is the identity for every value.
TEST(ClampTest, ClampingToItsOwnTypeIsIdentity)  // NOLINT
{
    CheckEveryValue<std::int8_t, std::int8_t>();
    CheckEveryValue<std::uint8_t, std::uint8_t>();
    CheckEveryValue<std::uint16_t, std::uint16_t>();

    static_assert(
        edt::ClampToAnotherTypeBounds<std::int64_t>(std::numeric_limits<std::int64_t>::min()) ==
        std::numeric_limits<std::int64_t>::min());
    static_assert(
        edt::ClampToAnotherTypeBounds<std::uint64_t>(std::numeric_limits<std::uint64_t>::max()) ==
        std::numeric_limits<std::uint64_t>::max());
}

// The extremes of the 64-bit types, where a bound that does not fit in the source
// would wrap if it were converted before being compared.
TEST(ClampTest, WideMixedSignednessExtremes)  // NOLINT
{
    constexpr auto kI64Min = std::numeric_limits<std::int64_t>::min();
    constexpr auto kI64Max = std::numeric_limits<std::int64_t>::max();
    constexpr auto kU64Max = std::numeric_limits<std::uint64_t>::max();

    // Unsigned source, signed bound: only the top half is clipped.
    static_assert(edt::ClampToAnotherTypeBounds<std::int64_t>(kU64Max) == static_cast<std::uint64_t>(kI64Max));
    static_assert(edt::ClampToAnotherTypeBounds<std::int64_t>(std::uint64_t{0}) == 0U);

    // Signed source, unsigned bound: only the negatives are clipped.
    static_assert(edt::ClampToAnotherTypeBounds<std::uint64_t>(kI64Min) == 0);
    static_assert(edt::ClampToAnotherTypeBounds<std::uint64_t>(std::int64_t{-1}) == 0);
    static_assert(edt::ClampToAnotherTypeBounds<std::uint64_t>(kI64Max) == kI64Max);
}
