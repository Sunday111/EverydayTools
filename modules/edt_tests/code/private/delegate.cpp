#include "edt/delegate.hpp"

#include "gtest/gtest.h"

namespace
{
int Doubled(int x)
{
    return x * 2;
}

struct Counter
{
    int Add(int x)
    {
        total += x;
        return total;
    }

    int operator()(int x) { return x + 1; }

    int total = 0;
};
}  // namespace

TEST(DelegateTest, StartsUnbound)
{
    const edt::Delegate<int(int)> d;
    EXPECT_FALSE(d.IsBound());
    EXPECT_FALSE(static_cast<bool>(d));
}

TEST(DelegateTest, BindsAFreeFunction)
{
    edt::Delegate<int(int)> d;
    d.Bind<&Doubled>();
    EXPECT_TRUE(d.IsBound());
    EXPECT_EQ(d.Invoke(21), 42);
    EXPECT_EQ(d(21), 42);
}

TEST(DelegateTest, BindsAMethod)
{
    Counter counter;
    edt::Delegate<int(int)> d;
    d.Bind<Counter, &Counter::Add>(&counter);
    EXPECT_EQ(d.Invoke(2), 2);
    EXPECT_EQ(d.Invoke(3), 5);
    EXPECT_EQ(counter.total, 5);
}

TEST(DelegateTest, BindsAFunctor)
{
    Counter counter;
    edt::Delegate<int(int)> d;
    d.Bind(counter);
    EXPECT_EQ(d.Invoke(41), 42);
}

TEST(DelegateTest, ResetUnbinds)
{
    edt::Delegate<int(int)> d;
    d.Bind<&Doubled>();
    d.Reset();
    EXPECT_FALSE(d.IsBound());
}

TEST(DelegateTest, SupportsVoidReturn)
{
    int seen = 0;
    auto set = [&seen](int x)
    {
        seen = x;
    };
    edt::Delegate<void(int)> d;
    d.Bind(set);
    d.Invoke(7);
    EXPECT_EQ(seen, 7);
}
