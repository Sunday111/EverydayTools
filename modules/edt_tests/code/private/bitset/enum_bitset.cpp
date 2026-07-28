#include "edt/bitset/enum_bitset.hpp"

#include <cstdint>

#include "edt/bitset/bitset_utilities.hpp"
#include "gtest/gtest.h"

namespace
{
enum class Flags : std::uint8_t
{
    None = 0,
    A = 1 << 0,
    B = 1 << 1,
    C = 1 << 2,
};

enum class NotAFlagSet : std::uint8_t
{
    Value = 1,
};
}  // namespace

template <>
struct edt::enable_enum_bitset<Flags>
{
    static constexpr bool value = true;
};

TEST(EnumBitsetTest, OnlyOptedInEnumsGetTheOperators)
{
    static_assert(edt::EnumBitset<Flags>);
    static_assert(!edt::EnumBitset<NotAFlagSet>);
    static_assert(!edt::EnumBitset<int>);
}

TEST(EnumBitsetTest, BitwiseOperators)
{
    static_assert((Flags::A | Flags::B) != Flags::A);
    static_assert(((Flags::A | Flags::B) & Flags::A) == Flags::A);
    static_assert(((Flags::A | Flags::B) & Flags::C) == Flags::None);
    static_assert((Flags::A ^ Flags::A) == Flags::None);
    static_assert((Flags::A << 1) == Flags::B);
    static_assert((Flags::B >> 1) == Flags::A);
    static_assert((~Flags::None & Flags::A) == Flags::A);
}

TEST(EnumBitsetTest, CompoundAssignment)
{
    Flags f = Flags::A;
    f |= Flags::B;
    EXPECT_EQ(edt::GetSetFlagsCount(f), 2U);

    f &= Flags::A;
    EXPECT_EQ(f, Flags::A);

    f ^= Flags::A;
    EXPECT_EQ(f, Flags::None);

    f = Flags::A;
    f <<= 2;
    EXPECT_EQ(f, Flags::C);
    f >>= 2;
    EXPECT_EQ(f, Flags::A);
}

TEST(EnumBitsetTest, FlagIsSet)
{
    constexpr Flags both = Flags::A | Flags::C;
    static_assert(edt::FlagIsSet(both, Flags::A));
    static_assert(edt::FlagIsSet(both, Flags::C));
    static_assert(!edt::FlagIsSet(both, Flags::B));
}
