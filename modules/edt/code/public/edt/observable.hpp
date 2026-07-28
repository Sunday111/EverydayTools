#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "edt/exception/call_and_rethrow.hpp"
#include "edt/exception/throw_if_failed.hpp"

namespace edt
{
template <typename T>
using RawPtr = T*;

template <typename Derived, typename Listener, template <typename> typename StorePtr = RawPtr>
class Observable
{
public:
    void Subscribe(StorePtr<Listener> listener)
    {
        CallAndRethrow(
            [&]
            {
                auto it = std::ranges::lower_bound(listeners_, listener);
                ThrowIfFailed(it == listeners_.end() || *it != listener, "This listener is already registered");
                listeners_.insert(it, std::move(listener));
            });
    }

    void Unsubscribe(const StorePtr<Listener>& listener)
    {
        CallAndRethrow(
            [&]
            {
                auto it = std::ranges::lower_bound(listeners_, listener);
                ThrowIfFailed(it != listeners_.end() && *it == listener, "Could not find listener");
                listeners_.erase(it);
            });
    }

protected:
    template <typename F>
    void ForEachListener(F&& f)
    {
        for (auto& listener : listeners_)
        {
            f(listener);
        }
    }

private:
    std::vector<StorePtr<Listener>> listeners_;
};
}  // namespace edt
