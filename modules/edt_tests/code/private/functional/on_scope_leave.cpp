#include "edt/functional/on_scope_leave.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

TEST(OnScopeLeaveTest, RunsAtScopeExit)
{
    int calls = 0;

    {
        auto guard = edt::OnScopeLeave([&] { ++calls; });
        EXPECT_EQ(calls, 0);
    }

    EXPECT_EQ(calls, 1);
}

TEST(OnScopeLeaveTest, RunsWhenScopeIsLeftByException)
{
    int calls = 0;

    try
    {
        auto guard = edt::OnScopeLeave([&] { ++calls; });
        throw std::runtime_error("leaving");
    }
    catch (const std::runtime_error&)
    {
    }

    EXPECT_EQ(calls, 1);
}

TEST(OnScopeLeaveTest, MovedFromGuardDoesNotRun)
{
    int calls = 0;

    {
        auto guard = edt::OnScopeLeave([&] { ++calls; });
        auto moved = std::move(guard);
        EXPECT_EQ(calls, 0);
    }

    EXPECT_EQ(calls, 1);
}

TEST(OnScopeLeaveTest, AcceptsCallableByLvalueReference)
{
    int calls = 0;
    auto increment = [&]
    {
        ++calls;
    };

    {
        auto guard = edt::OnScopeLeave(increment);
    }

    EXPECT_EQ(calls, 1);
}

TEST(OnScopeLeaveTest, GuardsRunInReverseOrderOfCreation)
{
    std::vector<int> order;

    {
        auto first = edt::OnScopeLeave([&] { order.push_back(1); });
        auto second = edt::OnScopeLeave([&] { order.push_back(2); });
    }

    EXPECT_EQ(order, (std::vector<int>{2, 1}));
}
