#pragma once

#include <vector>

#include "EverydayTools\Exception\ThrowIfFailed.h"

template<typename T>
using RawPtr = T*;

template
<
    typename Derived,
    typename Listener,
    template<typename>
    typename StorePtr = RawPtr
>
class Observable {
public:
    void Subscribe(StorePtr<Listener> listener) {
        CallAndRethrow("Observable::Subscribe", [&] {
            auto it = std::lower_bound(m_listeners.begin(), m_listeners.end(), listener);
            edt::ThrowIfFailed(
                it == m_listeners.end() || *it != listener,
                "This listener already registered");
            m_listeners.insert(it, std::move(listener));
        });
    }

    void Unsubscribe(StorePtr<Listener> listener) {
        CallAndRethrow("Observable::Unsubscribe", [&] {
            auto it = std::lower_bound(m_listeners.begin(), m_listeners.end(), listener);
            edt::ThrowIfFailed(
                it != m_listeners.end() && *it == listener,
                "Could not find listener");
            m_listeners.erase(it);
        });
    }

protected:
    template<typename F>
    void ForEachListener(F&& f) {
        for (auto& listener : m_listeners) {
            f(listener);
        }
    }

private:
    std::vector<StorePtr<Listener>> m_listeners;
};