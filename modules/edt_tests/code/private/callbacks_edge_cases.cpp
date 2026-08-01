#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "edt/delegate.hpp"
#include "edt/observable.hpp"
#include "gtest/gtest.h"

namespace
{
class Listener
{
public:
    int notifications = 0;
};

class Source : public edt::Observable<Source, Listener>
{
public:
    void Notify()
    {
        ForEachListener([](Listener* listener) { ++listener->notifications; });
    }

    [[nodiscard]] size_t NotifyAndCount()
    {
        size_t count = 0;
        ForEachListener([&count](Listener*) { ++count; });
        return count;
    }
};

int Doubled(int value)
{
    return value * 2;
}

struct Counter
{
    int calls = 0;
    int operator()(int value)
    {
        ++calls;
        return value + calls;
    }
};
}  // namespace

// Every subscriber is notified exactly once per event, however many there are and
// in whatever order they were added. The container is kept sorted internally, so
// notification order follows addresses rather than subscription order - the count
// is the part callers can rely on.
TEST(ObservableEdgeTest, EachListenerIsNotifiedExactlyOncePerEvent)  // NOLINT
{
    constexpr size_t kCount = 16;
    std::vector<std::unique_ptr<Listener>> listeners;
    for (size_t i = 0; i != kCount; ++i) listeners.push_back(std::make_unique<Listener>());

    Source source;
    // Subscribe in a scrambled order so a dependence on insertion order would show.
    for (size_t i = 0; i != kCount; ++i) source.Subscribe(listeners[(i * 7) % kCount].get());

    source.Notify();
    source.Notify();

    for (size_t i = 0; i != kCount; ++i) EXPECT_EQ(listeners[i]->notifications, 2) << "i = " << i;
}

TEST(ObservableEdgeTest, NoSubscribersIsANoOp)  // NOLINT
{
    Source source;
    EXPECT_EQ(source.NotifyAndCount(), 0U);
    EXPECT_NO_THROW(source.Notify());
}

// Removing from the front, middle and back all have to keep the remaining set intact.
TEST(ObservableEdgeTest, UnsubscribeFromAnyPositionKeepsTheRest)  // NOLINT
{
    constexpr size_t kCount = 8;
    std::vector<std::unique_ptr<Listener>> listeners;
    for (size_t i = 0; i != kCount; ++i) listeners.push_back(std::make_unique<Listener>());

    // Sorted by address, so "first" and "last" are well defined for this test.
    std::vector<Listener*> by_address;
    by_address.reserve(kCount);
    for (const auto& listener : listeners) by_address.push_back(listener.get());
    std::ranges::sort(by_address);

    for (const size_t victim : {size_t{0}, kCount / 2, kCount - 1})
    {
        Source source;
        for (Listener* listener : by_address) source.Subscribe(listener);

        source.Unsubscribe(by_address[victim]);
        EXPECT_EQ(source.NotifyAndCount(), kCount - 1) << "victim = " << victim;

        // Removing it a second time is an error, not a silent no-op.
        EXPECT_THROW(source.Unsubscribe(by_address[victim]), std::runtime_error) << "victim = " << victim;
    }
}

// Subscribing again after unsubscribing is allowed - the rejection of duplicates
// must not leave anything behind that blocks re-registration.
TEST(ObservableEdgeTest, ResubscribeAfterUnsubscribe)  // NOLINT
{
    Listener listener;
    Source source;

    for (int round = 0; round != 3; ++round)
    {
        source.Subscribe(&listener);
        EXPECT_THROW(source.Subscribe(&listener), std::runtime_error) << "round = " << round;
        EXPECT_EQ(source.NotifyAndCount(), 1U) << "round = " << round;
        source.Unsubscribe(&listener);
        EXPECT_EQ(source.NotifyAndCount(), 0U) << "round = " << round;
    }
}

// A failed Subscribe or Unsubscribe must leave the subscriber set unchanged.
TEST(ObservableEdgeTest, FailedCallsDoNotDisturbTheSet)  // NOLINT
{
    Listener first;
    Listener second;
    Listener never_subscribed;

    Source source;
    source.Subscribe(&first);
    source.Subscribe(&second);

    EXPECT_THROW(source.Subscribe(&first), std::runtime_error);
    EXPECT_THROW(source.Unsubscribe(&never_subscribed), std::runtime_error);

    EXPECT_EQ(source.NotifyAndCount(), 2U);
}

// Rebinding replaces the previous target rather than adding to it, whichever kind
// of callable each side is.
TEST(DelegateEdgeTest, RebindingReplacesTheTarget)  // NOLINT
{
    edt::Delegate<int(int)> delegate;
    EXPECT_FALSE(delegate.IsBound());

    delegate.Bind<&Doubled>();
    EXPECT_EQ(delegate.Invoke(21), 42);

    Counter counter;
    delegate.Bind(counter);
    EXPECT_EQ(delegate.Invoke(10), 11);
    EXPECT_EQ(counter.calls, 1);

    // Back to the free function: the functor must no longer be reachable.
    delegate.Bind<&Doubled>();
    EXPECT_EQ(delegate.Invoke(10), 20);
    EXPECT_EQ(counter.calls, 1);
}

// Reset makes it unbound again, and Reset on an already unbound delegate is fine.
TEST(DelegateEdgeTest, ResetIsIdempotent)  // NOLINT
{
    edt::Delegate<int(int)> delegate;
    delegate.Reset();
    EXPECT_FALSE(delegate.IsBound());

    delegate.Bind<&Doubled>();
    ASSERT_TRUE(delegate.IsBound());

    delegate.Reset();
    delegate.Reset();
    EXPECT_FALSE(delegate.IsBound());
    EXPECT_FALSE(static_cast<bool>(delegate));
}

// The delegate holds the functor by address, so mutations through either the
// original or the delegate are seen by both.
TEST(DelegateEdgeTest, FunctorIsHeldByReference)  // NOLINT
{
    Counter counter;
    edt::Delegate<int(int)> delegate;
    delegate.Bind(counter);

    EXPECT_EQ(delegate.Invoke(0), 1);
    EXPECT_EQ(delegate.Invoke(0), 2);
    EXPECT_EQ(counter.calls, 2);

    counter.calls = 10;
    EXPECT_EQ(delegate.Invoke(0), 11);
}

// A copied delegate points at the same target; copying does not clone the functor.
TEST(DelegateEdgeTest, CopySharesTheSameTarget)  // NOLINT
{
    Counter counter;
    edt::Delegate<int(int)> original;
    original.Bind(counter);

    edt::Delegate<int(int)> copy = original;
    EXPECT_TRUE(copy.IsBound());

    EXPECT_EQ(original.Invoke(0), 1);
    EXPECT_EQ(copy.Invoke(0), 2);
    EXPECT_EQ(counter.calls, 2);

    // Resetting one leaves the other bound.
    original.Reset();
    EXPECT_FALSE(original.IsBound());
    EXPECT_TRUE(copy.IsBound());
    EXPECT_EQ(copy.Invoke(0), 3);
}

// Argument and return types that are not trivially copyable still pass through.
TEST(DelegateEdgeTest, NonTrivialArgumentsAndReturns)  // NOLINT
{
    edt::Delegate<std::string(std::string)> delegate;
    auto exclaim = [](const std::string& text)
    {
        return text + "!";
    };
    delegate.Bind(exclaim);

    EXPECT_EQ(delegate.Invoke("hello"), "hello!");
    EXPECT_EQ(delegate(""), "!");
}

// A zero-argument delegate is the degenerate signature.
TEST(DelegateEdgeTest, NoArgumentSignature)  // NOLINT
{
    int calls = 0;
    auto increment = [&calls]
    {
        ++calls;
    };

    edt::Delegate<void()> delegate;
    delegate.Bind(increment);
    delegate.Invoke();
    delegate();

    EXPECT_EQ(calls, 2);
}
