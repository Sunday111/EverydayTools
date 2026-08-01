#include "edt/size.hpp"

#include <array>
#include <cstdint>

#include "gtest/gtest.h"

static_assert(edt::SizeInBytes<std::uint8_t> == 1);
static_assert(edt::SizeInBytes<std::uint64_t> == 8);
static_assert(edt::SizeInBits<std::uint8_t> == 8);
static_assert(edt::SizeInBits<std::uint16_t> == 16);
static_assert(edt::SizeInBits<std::uint32_t> == 32);
static_assert(edt::SizeInBits<std::uint64_t> == 64);

// Signedness does not change the width.
static_assert(edt::SizeInBits<std::int8_t> == edt::SizeInBits<std::uint8_t>);
static_assert(edt::SizeInBits<std::int64_t> == edt::SizeInBits<std::uint64_t>);

// It is sizeof, so padding counts and the minimum is one byte.
namespace
{
struct Empty
{
};

struct Padded
{
    std::uint8_t a;
    std::uint32_t b;
};
}  // namespace

static_assert(edt::SizeInBytes<Empty> == 1);
static_assert(edt::SizeInBits<Empty> == 8);
static_assert(edt::SizeInBytes<Padded> == sizeof(Padded));
static_assert(edt::SizeInBits<Padded> == sizeof(Padded) * 8);
static_assert(edt::SizeInBits<Padded> > edt::SizeInBits<std::uint8_t> + edt::SizeInBits<std::uint32_t>);

// Arrays and references behave as sizeof does - the latter reports the referent.
static_assert(edt::SizeInBytes<std::array<std::uint16_t, 4>> == 8);
static_assert(edt::SizeInBits<std::array<std::uint16_t, 4>> == 64);
static_assert(edt::SizeInBytes<std::uint32_t&> == sizeof(std::uint32_t));

// The relationship that callers actually rely on: bits are bytes times CHAR_BIT,
// for every type.
template <typename T>
constexpr bool BitsMatchBytes()
{
    return edt::SizeInBits<T> == edt::SizeInBytes<T> * 8;
}

static_assert(BitsMatchBytes<std::uint8_t>());
static_assert(BitsMatchBytes<std::int64_t>());
static_assert(BitsMatchBytes<Empty>());
static_assert(BitsMatchBytes<Padded>());
static_assert(BitsMatchBytes<double>());

TEST(SizeTest, IsUsableAsARuntimeValue)  // NOLINT
{
    EXPECT_EQ(edt::SizeInBits<std::uint8_t>, 8U);
    EXPECT_EQ(edt::SizeInBytes<Padded>, sizeof(Padded));
}
