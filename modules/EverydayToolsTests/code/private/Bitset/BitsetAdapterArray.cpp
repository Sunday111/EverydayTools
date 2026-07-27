#include <array>
#include <cstdint>
#include <span>
#include <utility>

#include "EverydayTools/Bitset/BitsetAdapter.hpp"
#include "gtest/gtest.h"

namespace
{
// Applies SetRange(begin, end, value) to a fresh backing pre-filled to `initial`
// and asserts every bit against the obvious reference: bits inside [begin, end)
// become `value`, everything else keeps `initial`.
template <typename Part, size_t NParts>
void CheckSetRange(const bool initial, const size_t begin, const size_t end, const bool value)
{
    constexpr size_t kBits = NParts * sizeof(Part) * 8;
    std::array<Part, NParts> parts{};
    parts.fill(initial ? static_cast<Part>(~Part{0}) : Part{0});

    edt::BitsetAdapter adapter{std::span(parts)};
    adapter.SetRange(begin, end, value);

    for (size_t i = 0; i != kBits; ++i)
    {
        const bool expected = (i >= begin && i < end) ? value : initial;
        ASSERT_EQ(adapter.Get(i), expected)
            << "part_bytes=" << sizeof(Part) << " nparts=" << NParts << " initial=" << initial << " value=" << value
            << " range=[" << begin << "," << end << ") bit=" << i;
    }
}

// Every (begin, end) pair, over empty and full backing, setting to 0 and 1.
template <typename Part, size_t NParts>
void ExhaustiveSetRange()
{
    constexpr size_t kBits = NParts * sizeof(Part) * 8;
    for (const bool initial : {false, true})
    {
        for (const bool value : {false, true})
        {
            for (size_t begin = 0; begin <= kBits; ++begin)
            {
                for (size_t end = begin; end <= kBits; ++end)
                {
                    CheckSetRange<Part, NParts>(initial, begin, end, value);
                }
            }
        }
    }
}

// The constexpr constructor lets a whole build-set-read sequence run at compile
// time over a std::array backing wrapped in a static-extent span.
constexpr uint32_t ConstexprArraySetRange()
{
    std::array<uint8_t, 3> arr{};
    edt::BitsetAdapter adapter{std::span(arr)};
    adapter.SetRange(3, 11, true);  // spans the first part boundary
    adapter.Set(20, true);          // single-bit set in the last part
    uint32_t result = 0;
    for (size_t i = 0; i != 24; ++i)
    {
        if (adapter.Get(i)) result |= (uint32_t{1} << i);
    }
    return result;
}
}  // namespace

// Bits [3, 11) set (0x7F8) plus bit 20 set (0x100000) => 0x1007F8.
static_assert(ConstexprArraySetRange() == 0x1007F8u);

// PartsCount()/Size() are constexpr for a static-extent adapter.
static_assert(
    []
    {
        std::array<uint16_t, 4> arr{};
        edt::BitsetAdapter adapter{std::span(arr)};
        return adapter.PartsCount() == 4 && adapter.Size() == 64;
    }());
static_assert(edt::BitsetAdapter<uint16_t, 4>::kStaticExtent);

// 8-bit parts exercise every part-boundary combination cheaply: single-part,
// spanning 2-5 parts, boundary-aligned and mid-part starts and ends.
TEST(BitsetAdapterTest, SetRangeExhaustive8BitParts)  // NOLINT
{
    ExhaustiveSetRange<uint8_t, 5>();
}

// Wider parts confirm the logic is independent of part width.
TEST(BitsetAdapterTest, SetRangeExhaustive16BitParts)  // NOLINT
{
    ExhaustiveSetRange<uint16_t, 3>();
}

// 64-bit parts: hit part-aligned ends (64, 128), mid-part ends, spanning ranges,
// and single-bit ranges straddling a boundary. A boundary-aligned end is exactly
// the case that used to skip the trailing-part patch.
TEST(BitsetAdapterTest, SetRange64BitPartBoundaries)  // NOLINT
{
    using Part = uint64_t;
    constexpr size_t kBits = 3 * 64;
    const auto ranges = std::to_array<std::pair<size_t, size_t>>({
        {0, 0},
        {0, 1},
        {0, 64},
        {0, 65},
        {0, 128},
        {0, 192},
        {64, 128},
        {64, 192},
        {63, 64},
        {63, 65},
        {64, 65},
        {127, 129},
        {1, 191},
        {10, 190},
        {128, 192},
        {191, 192},
        {100, 100},
        {96, 160},
    });
    for (const bool initial : {false, true})
    {
        for (const bool value : {false, true})
        {
            for (const auto& [begin, end] : ranges)
            {
                CheckSetRange<Part, 3>(initial, begin, end, value);
            }
        }
    }
    static_cast<void>(kBits);
}

TEST(BitsetAdapterTest, SetGetRoundTrip)  // NOLINT
{
    std::array<uint32_t, 4> parts{};
    edt::BitsetAdapter adapter{std::span(parts)};
    constexpr size_t kBits = 4 * 32;

    for (size_t i = 0; i != kBits; ++i)
    {
        const bool value = (i * 7 + 3) % 5 == 0;
        adapter.Set(i, value);
    }
    for (size_t i = 0; i != kBits; ++i)
    {
        EXPECT_EQ(adapter.Get(i), (i * 7 + 3) % 5 == 0) << "bit=" << i;
    }
}
