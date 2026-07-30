#include <vector>

#include "edt/algorithm/compare.hpp"
#include "edt/math/float_range.hpp"
#include "edt/math/int_range.hpp"
#include "gtest/gtest.h"

namespace
{
// -Wfloat-equal is on, and these are exact-representable constants, so
// comparisons go through the library's own tolerance-based check.
template <std::floating_point T>
constexpr bool Same(T a, T b)
{
    return edt::AlmostEqual(a, b, static_cast<T>(1e-6));
}
}  // namespace

TEST(FloatRangeTest, BasicQueries)
{
    constexpr edt::FloatRange<float> r{.begin = -1.f, .end = 3.f};

    static_assert(Same(r.Extent(), 4.f));
    static_assert(r.Contains(0.f));
    static_assert(r.Contains(-1.f));
    static_assert(r.Contains(3.f));
    static_assert(!r.Contains(3.5f));
    static_assert(Same(r.Clamp(10.f), 3.f));
    static_assert(Same(r.Clamp(-10.f), -1.f));
}

TEST(FloatRangeTest, ShiftedAndEnlarged)
{
    constexpr edt::FloatRange<float> r{.begin = 0.f, .end = 1.f};

    constexpr auto shifted = r.Shifted(2.f);
    static_assert(Same(shifted.begin, 2.f));
    static_assert(Same(shifted.end, 3.f));

    constexpr auto enlarged = r.Enlarged(1.f);
    static_assert(Same(enlarged.begin, -1.f));
    static_assert(Same(enlarged.end, 2.f));
}

// A double range must not round its arguments through float.
TEST(FloatRangeTest, DoubleRangeKeepsPrecision)
{
    edt::FloatRange<double> r{.begin = 0.0, .end = 1.0};

    constexpr double tiny = 1e-300;
    r.Shift(tiny);
    EXPECT_GT(r.begin, 0.0) << "the shift was truncated through float";
    EXPECT_LT(r.begin, 1e-299);

    const edt::FloatRange<double> unit{.begin = 0.0, .end = 1.0};
    EXPECT_DOUBLE_EQ(unit.Clamp(0.1), 0.1);
}

TEST(FloatRange2DTest, Basics)
{
    constexpr auto r = edt::FloatRange2D<float>::FromMinMax(edt::Vec2f{0.f, 0.f}, edt::Vec2f{2.f, 4.f});

    static_assert(r.Contains(1.f, 1.f));
    static_assert(!r.Contains(3.f, 1.f));
    static_assert(Same(r.Extent().x(), 2.f));
    static_assert(Same(r.Extent().y(), 4.f));
    static_assert(Same(r.Min().x(), 0.f));
    static_assert(Same(r.Max().y(), 4.f));
    static_assert(Same(r.Uniform(edt::Vec2f{0.5f, 0.5f}).x(), 1.f));
    static_assert(Same(r.Uniform(edt::Vec2f{0.5f, 0.5f}).y(), 2.f));
}

// Contains() called an unqualified InRange that was never in scope, so it did
// not compile for any T.
TEST(IntRangeTest, Contains)
{
    constexpr edt::IntRange<int> r{.begin = 2, .end = 5};

    static_assert(r.Contains(2));
    static_assert(r.Contains(4));
    static_assert(!r.Contains(5));  // half-open
    static_assert(!r.Contains(1));
    static_assert(r.Extent() == 3);
}

TEST(IntRangeTest, SplitAndIterate)
{
    constexpr edt::IntRange<int> r{.begin = 0, .end = 10};

    constexpr auto halves = r.Split(4);
    static_assert(std::get<0>(halves).begin == 0);
    static_assert(std::get<0>(halves).end == 4);
    static_assert(std::get<1>(halves).begin == 4);
    static_assert(std::get<1>(halves).end == 10);

    std::vector<int> seen;
    for (const int i : edt::IntRange<int>{.begin = 1, .end = 4}.Iterable()) seen.push_back(i);
    EXPECT_EQ(seen, (std::vector<int>{1, 2, 3}));
}

TEST(IntRange2DTest, FromBeginAndExtent)
{
    constexpr auto r = edt::IntRange2D<int>::FromBeginAndExtent(edt::Vec2<int>{1, 2}, edt::Vec2<int>{3, 4});

    static_assert(r.x.begin == 1);
    static_assert(r.x.end == 4);
    static_assert(r.y.begin == 2);
    static_assert(r.y.end == 6);
    static_assert(r.Contains(2, 3));
    static_assert(!r.Contains(4, 3));
}
