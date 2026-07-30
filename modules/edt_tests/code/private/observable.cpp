#include "edt/observable.hpp"

#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"

namespace
{
struct Listener
{
    int calls = 0;
};

class Subject : public edt::Observable<Subject, Listener>
{
public:
    void Notify()
    {
        ForEachListener([](Listener* listener) { ++listener->calls; });
    }
};
}  // namespace

TEST(ObservableTest, NotifiesEverySubscriber)
{
    Subject subject;
    Listener a;
    Listener b;

    subject.Subscribe(&a);
    subject.Subscribe(&b);
    subject.Notify();

    EXPECT_EQ(a.calls, 1);
    EXPECT_EQ(b.calls, 1);
}

TEST(ObservableTest, RejectsDuplicateSubscription)
{
    Subject subject;
    Listener a;

    subject.Subscribe(&a);
    EXPECT_THROW(subject.Subscribe(&a), std::runtime_error);
}

TEST(ObservableTest, UnsubscribeStopsNotifications)
{
    Subject subject;
    Listener a;
    Listener b;

    subject.Subscribe(&a);
    subject.Subscribe(&b);
    subject.Unsubscribe(&a);
    subject.Notify();

    EXPECT_EQ(a.calls, 0);
    EXPECT_EQ(b.calls, 1);
}

TEST(ObservableTest, UnsubscribingSomethingUnknownThrows)
{
    Subject subject;
    Listener a;
    EXPECT_THROW(subject.Unsubscribe(&a), std::runtime_error);
}
