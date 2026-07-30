#include <string>
#include <vector>

#include "edt/concepts/callable.hpp"
#include "edt/functional/call_for_each_arg.hpp"
#include "edt/functional/wrap.hpp"
#include "gtest/gtest.h"

TEST(CallForEachArgTest, VisitsEveryArgumentInOrder)
{
    std::vector<int> seen;
    edt::CallForEachArg([&seen](int v) { seen.push_back(v); }, 1, 2, 3);
    EXPECT_EQ(seen, (std::vector<int>{1, 2, 3}));
}

TEST(WrapFunctionTest, PassesTheResultThroughTheWrapper)
{
    const auto wrapped = edt::WrapFunction([](int a, int b) { return a + b; }, [](int r) { return r * 10; });
    EXPECT_EQ(wrapped(1, 2), 30);
}

TEST(CallableConceptTest, MatchesSignature)
{
    const auto f = [](int a)
    {
        return a + 1;
    };
    static_assert(edt::Callable<decltype(f), int, int>);
    static_assert(!edt::Callable<decltype(f), std::string, int>);
    static_assert(!edt::Callable<decltype(f), int, std::string>);
}
