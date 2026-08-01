#include <cstdint>
#include <limits>
#include <stdexcept>
#include <utility>

#include "edt/exception/checked_cast.hpp"
#include "gtest/gtest.h"

namespace
{
// The interesting inputs for a conversion are the ends of both types' ranges and
// the values immediately either side of them - derived, not written out per pair.
template <typename From>
std::vector<From> InterestingValues()
{
    constexpr auto kLowest = std::numeric_limits<From>::lowest();
    constexpr auto kMax = std::numeric_limits<From>::max();

    std::vector<From> values{kLowest, kMax, From{0}, From{1}};
    if (kLowest != From{0})
    {
        values.push_back(static_cast<From>(kLowest + 1));
        values.push_back(static_cast<From>(-1));
    }
    values.push_back(static_cast<From>(kMax - 1));

    // The boundaries of every other type, where they are representable here.
    const auto add_if_representable = [&values](auto limit)
    {
        if (std::in_range<From>(limit)) values.push_back(static_cast<From>(limit));
    };
    add_if_representable(std::numeric_limits<std::int8_t>::lowest());
    add_if_representable(std::numeric_limits<std::int8_t>::max());
    add_if_representable(std::numeric_limits<std::uint8_t>::max());
    add_if_representable(std::numeric_limits<std::int16_t>::lowest());
    add_if_representable(std::numeric_limits<std::int16_t>::max());
    add_if_representable(std::numeric_limits<std::uint16_t>::max());
    add_if_representable(std::numeric_limits<std::int32_t>::lowest());
    add_if_representable(std::numeric_limits<std::int32_t>::max());
    add_if_representable(std::numeric_limits<std::uint32_t>::max());

    return values;
}

// std::in_range is the oracle: it decides whether the cast may pass, and the two
// must never disagree in either direction.
template <typename To, typename From>
void CheckOnePair()
{
    for (const From value : InterestingValues<From>())
    {
        if (std::in_range<To>(value))
        {
            To result{};
            ASSERT_NO_THROW(result = edt::CheckedCast<To>(value));
            // The value survived, so casting it back must reproduce the input.
            EXPECT_TRUE(std::cmp_equal(result, value));
        }
        else
        {
            EXPECT_THROW((void)edt::CheckedCast<To>(value), std::runtime_error);
        }
    }
}

template <typename To, typename... From>
void CheckAgainstEverySource()
{
    (CheckOnePair<To, From>(), ...);
}

template <typename... To>
void CheckEveryPair()
{
    (CheckAgainstEverySource<
         To,
         std::int8_t,
         std::uint8_t,
         std::int16_t,
         std::uint16_t,
         std::int32_t,
         std::uint32_t,
         std::int64_t,
         std::uint64_t>(),
     ...);
}
}  // namespace

// All 64 ordered pairs of the standard fixed-width integer types.
TEST(CheckedCastTest, AgreesWithStdInRangeForEveryTypePair)  // NOLINT
{
    CheckEveryPair<
        std::int8_t,
        std::uint8_t,
        std::int16_t,
        std::uint16_t,
        std::int32_t,
        std::uint32_t,
        std::int64_t,
        std::uint64_t>();
}

// Every value of the narrow types, not just the interesting ones.
TEST(CheckedCastTest, AgreesWithStdInRangeForEveryNarrowValue)  // NOLINT
{
    for (std::uint32_t pattern = 0; pattern != 65536; ++pattern)
    {
        const auto as_signed = static_cast<std::int16_t>(static_cast<std::uint16_t>(pattern));
        const auto as_unsigned = static_cast<std::uint16_t>(pattern);

        EXPECT_EQ(
            std::in_range<std::int8_t>(as_signed),
            [&]
            {
                try
                {
                    (void)edt::CheckedCast<std::int8_t>(as_signed);
                    return true;
                }
                catch (const std::runtime_error&)
                {
                    return false;
                }
            }())
            << "pattern = " << pattern;

        EXPECT_EQ(
            std::in_range<std::uint8_t>(as_unsigned),
            [&]
            {
                try
                {
                    (void)edt::CheckedCast<std::uint8_t>(as_unsigned);
                    return true;
                }
                catch (const std::runtime_error&)
                {
                    return false;
                }
            }())
            << "pattern = " << pattern;
    }
}

// Converting to a type that cannot represent the source at all still succeeds for
// the values that do fit, rather than rejecting the whole source type.
TEST(CheckedCastTest, NarrowingKeepsTheRepresentableValues)  // NOLINT
{
    static_assert(edt::CheckedCast<std::uint8_t>(std::uint64_t{255}) == 255);
    static_assert(edt::CheckedCast<std::int8_t>(std::int64_t{-128}) == -128);
    EXPECT_THROW((void)edt::CheckedCast<std::uint8_t>(std::uint64_t{256}), std::runtime_error);
    EXPECT_THROW((void)edt::CheckedCast<std::int8_t>(std::int64_t{-129}), std::runtime_error);
}
