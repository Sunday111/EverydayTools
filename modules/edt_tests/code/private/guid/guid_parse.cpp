#include <random>
#include <string>
#include <string_view>

#include "edt/guid.hpp"
#include "edt/string/char_util.hpp"
#include "gtest/gtest.h"

namespace
{
std::string ToString(const edt::GUID& guid)
{
    const auto chars = guid.ToCharArray();
    return {chars.data(), chars.size()};
}

std::string WithCase(std::string_view text, bool upper)
{
    std::string result(text);
    for (char& c : result) c = upper ? edt::ToUpper(c) : edt::ToLower(c);
    return result;
}
}  // namespace

// The parser and the formatter each carry their own hand-written table mapping
// nibbles to string positions. Nothing else checks that the two agree, so a
// transposition in either would go unnoticed - a round trip catches it.
TEST(GuidParseTest, FormatThenParseIsIdentity)  // NOLINT
{
    std::mt19937_64 generator(20260731);

    for (size_t attempt = 0; attempt != 2000; ++attempt)
    {
        const edt::GUID original{.part1 = generator(), .part2 = generator()};
        const edt::GUID reparsed = edt::GUID::Create(ToString(original));
        EXPECT_EQ(reparsed, original) << "text = " << ToString(original);
    }
}

// Each nibble position must move independently: setting one bit of the value
// changes exactly one character of the text.
TEST(GuidParseTest, EveryNibblePositionIsDistinct)  // NOLINT
{
    const std::string zero = ToString(edt::GUID{});

    for (size_t bit = 0; bit != 128; ++bit)
    {
        edt::GUID guid{};
        if (bit < 64)
        {
            guid.part1 = edt::u64{1} << bit;
        }
        else
        {
            guid.part2 = edt::u64{1} << (bit - 64);
        }

        const std::string text = ToString(guid);
        ASSERT_EQ(text.size(), zero.size());

        size_t differences = 0;
        for (size_t i = 0; i != text.size(); ++i) differences += (text[i] != zero[i]) ? 1U : 0U;
        EXPECT_EQ(differences, 1U) << "bit = " << bit << ", text = " << text;

        EXPECT_EQ(edt::GUID::Create(text), guid) << "bit = " << bit;
    }
}

TEST(GuidParseTest, Extremes)  // NOLINT
{
    constexpr auto zero = edt::GUID::Create("00000000-0000-0000-0000-000000000000");
    static_assert(zero.part1 == 0);
    static_assert(zero.part2 == 0);
    EXPECT_EQ(ToString(zero), "00000000-0000-0000-0000-000000000000");

    constexpr auto ones = edt::GUID::Create("FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF");
    static_assert(ones.part1 == ~edt::u64{0});
    static_assert(ones.part2 == ~edt::u64{0});
    EXPECT_EQ(ToString(ones), "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF");

    // The default-constructed value is the all-zero GUID, not an unspecified one.
    static_assert(edt::GUID{} == zero);
}

// Parsing accepts either case; formatting always produces upper case.
TEST(GuidParseTest, IsCaseInsensitiveOnInput)  // NOLINT
{
    constexpr std::string_view kText = "22345200-abe8-4f60-90c8-0d43c5f6c0f6";

    const edt::GUID lower = edt::GUID::Create(WithCase(kText, false));
    const edt::GUID upper = edt::GUID::Create(WithCase(kText, true));

    EXPECT_EQ(lower, upper);
    EXPECT_EQ(ToString(lower), WithCase(kText, true));
}

// The braced spelling is the other length Create accepts; it must land on the
// same value as the bare one.
TEST(GuidParseTest, AcceptsTheBracedForm)  // NOLINT
{
    constexpr std::string_view kBare = "22345200-ABE8-4F60-90C8-0D43C5F6C0F6";

    constexpr auto bare = edt::GUID::Create(kBare);
    constexpr auto braced = edt::GUID::Create("{22345200-ABE8-4F60-90C8-0D43C5F6C0F6}");
    static_assert(bare == braced);

    EXPECT_EQ(bare, braced);
    EXPECT_EQ(ToString(braced), kBare);
}

TEST(GuidParseTest, SeparatorsSitWhereTheyBelong)  // NOLINT
{
    const std::string text = ToString(edt::GUID{.part1 = 0x0123456789ABCDEF, .part2 = 0xFEDCBA9876543210});

    ASSERT_EQ(text.size(), edt::GUID::usualStringLength);
    for (size_t i = 0; i != text.size(); ++i)
    {
        const bool is_separator = (i == 8 || i == 13 || i == 18 || i == 23);
        EXPECT_EQ(text[i] == '-', is_separator) << "i = " << i << ", text = " << text;
    }
}

// Ordering follows part1 first, then part2 - the member declaration order the
// defaulted spaceship uses.
TEST(GuidParseTest, OrderingIsLexicographicOverTheParts)  // NOLINT
{
    constexpr edt::GUID low{.part1 = 1, .part2 = 0};
    constexpr edt::GUID high{.part1 = 1, .part2 = 1};
    constexpr edt::GUID higher{.part1 = 2, .part2 = 0};

    static_assert(low < high);
    static_assert(high < higher);
    static_assert(low < higher);
}
