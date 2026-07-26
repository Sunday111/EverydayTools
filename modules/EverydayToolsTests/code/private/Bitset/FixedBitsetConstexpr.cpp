#include <cstddef>
#include <cstdint>

#include "EverydayTools/Bitset/FixedBitset.hpp"

namespace
{
// With the constexpr adapter constructor a FixedBitset is a compile-time bitset:
// this whole set/fill/read sequence runs inside a constant expression.
constexpr bool ConstexprFixedBitset()
{
    edt::FixedBitset<40> bitset;
    bitset.SetRange(3, 11, true);  // bits [3, 11)
    bitset.Set(20, true);          // single bit

    for (size_t i = 0; i != bitset.Size(); ++i)
    {
        const bool expected = (i >= 3 && i < 11) || i == 20;
        if (bitset.Get(i) != expected) return false;
    }

    bitset.Fill(true);
    for (size_t i = 0; i != bitset.Size(); ++i)
    {
        if (!bitset.Get(i)) return false;
    }

    bitset.SetRange(0, bitset.Size(), false);
    for (size_t i = 0; i != bitset.Size(); ++i)
    {
        if (bitset.Get(i)) return false;
    }

    return true;
}
}  // namespace

static_assert(ConstexprFixedBitset());

// Static geometry queries are constant expressions too.
static_assert(edt::FixedBitset<40>::Size() == 40);
static_assert(edt::FixedBitset<40>::PartsCount() == 5);  // 40 bits => 5 uint8_t parts
static_assert(edt::FixedBitset<64>::Size() == 64);
static_assert(edt::FixedBitset<64>::PartsCount() == 1);  // one uint64_t part
