#pragma once

#include <cassert>
#include <type_traits>

namespace edt
{
template <class T, typename Traits>
class IntrusivePtr
{
    template <typename U>
    using can_convert_from = std::enable_if_t<std::is_convertible_v<U*, T*>>;

public:
    IntrusivePtr() : m_p(nullptr) {}

    IntrusivePtr(std::nullptr_t) : m_p(nullptr) {}

    IntrusivePtr(T* p) : m_p(p) { AddReference(); }

    IntrusivePtr(const IntrusivePtr& ip) { CopyFrom<false, false>(ip); }

    IntrusivePtr(IntrusivePtr&& ip) { MoveFrom<false>(ip); }

    template <typename U, typename Enable = can_convert_from<U>>
    IntrusivePtr(const IntrusivePtr<U, Traits>& ip) : m_p(ip.m_p)
    {
        AddReference();
    }

    template <typename U, typename Enable = can_convert_from<U>>
    IntrusivePtr(IntrusivePtr<U, Traits>&& ip) : m_p(ip.m_p)
    {
        ip.m_p = nullptr;
    }

    ~IntrusivePtr() { ReleaseReference<false>(); }

    void reset() { ReleaseReference<true>(); }

    IntrusivePtr& swap(IntrusivePtr& p)
    {
        auto tmp = m_p;
        m_p = p.m_p;
        p.m_p = tmp;
        return *this;
    }

    T* Get() const { return m_p; }

    template <typename U>
    IntrusivePtr<U, Traits> StaticCast() const
    {
        typename IntrusivePtr<U, Traits>::StaticCastTag tag;
        return IntrusivePtr<U, Traits>(*this, tag);
    }

    template <typename U>
    IntrusivePtr<U, Traits> DynamicCast() const
    {
        typename IntrusivePtr<U, Traits>::DynamicCastTag tag;
        return IntrusivePtr<U, Traits>(*this, tag);
    }

    IntrusivePtr& operator=(const IntrusivePtr& ptr)
    {
        CopyFrom<true, true>(ptr);
        return *this;
    }

    template <typename U, typename Enable = can_convert_from<U>>
    IntrusivePtr& operator=(const IntrusivePtr<U, Traits>& ptr)
    {
        CopyFrom<true, true>(ptr);
        return *this;
    }

    IntrusivePtr& operator=(IntrusivePtr&& ptr)
    {
        MoveFrom<true>(ptr);
        return *this;
    }

    template <typename U, typename Enable = can_convert_from<U>>
    IntrusivePtr& operator=(IntrusivePtr<U, Traits>&& ptr)
    {
        MoveFrom<true>(ptr);
        return *this;
    }

    bool operator==(std::nullptr_t) const { return m_p == nullptr; }

    bool operator!=(std::nullptr_t) const { return m_p != nullptr; }

    template <typename U, typename Enable = can_convert_from<U>>
    bool operator==(const IntrusivePtr<U, Traits>& p) const
    {
        return m_p == p.m_p;
    }

    template <typename U, typename Enable = can_convert_from<U>>
    bool operator!=(const IntrusivePtr<U, Traits>& p)
    {
        return m_p != p.m_p;
    }

    template <typename U, typename Enable = can_convert_from<U>>
    bool operator<(const IntrusivePtr<U, Traits>& p) const
    {
        return m_p < p.m_p;
    }

    T* operator->() const
    {
        assert(m_p != nullptr);
        return m_p;
    }

    T& operator*() const
    {
        assert(m_p != nullptr);
        return *m_p;
    }

    operator bool() const { return m_p != nullptr; }

    template <typename... Args>
    static IntrusivePtr MakeInstance(Args&&... args)
    {
        return IntrusivePtr(new T(std::forward<Args>(args)...));
    }

protected:
    template <bool releasePrev, bool comparePointers, typename U>
    void CopyFrom(const IntrusivePtr<U, Traits>& that)
    {
        if constexpr (comparePointers)
        {
            if (m_p == that.m_p)
            {
                return;
            }
        }

        if constexpr (releasePrev)
        {
            ReleaseReference<false>();
        }
        m_p = that.m_p;
        AddReference();
    }

    template <bool releasePrev, typename U>
    void MoveFrom(IntrusivePtr<U, Traits>& that)
    {
        if constexpr (releasePrev)
        {
            ReleaseReference<false>();
        }
        m_p = that.m_p;
        that.m_p = nullptr;
    }

    inline void AddReference()
    {
        if (m_p != nullptr)
        {
            Traits::AddReference(m_p);
        }
    }

    template <bool set_to_null>
    inline void ReleaseReference()
    {
        if (m_p != nullptr)
        {
            Traits::ReleaseReference(m_p);
            if constexpr (set_to_null)
            {
                m_p = nullptr;
            }
        }
    }

private:
    template <typename, typename>
    friend class IntrusivePtr;

    struct StaticCastTag
    {
    };
    struct DynamicCastTag
    {
    };

    template <typename U>
    IntrusivePtr(const IntrusivePtr<U, Traits>& ip, StaticCastTag) : m_p(static_cast<T*>(ip.m_p))
    {
        AddReference();
    }

    template <typename U>
    IntrusivePtr(const IntrusivePtr<U, Traits>& ip, DynamicCastTag) : m_p(dynamic_cast<T*>(ip.m_p))
    {
        AddReference();
    }

    T* m_p;
};
}  // namespace edt

namespace std
{
template <typename U, typename T, typename Traits>
edt::IntrusivePtr<U, typename Traits> static_pointer_cast(const edt::IntrusivePtr<T, typename Traits>& ip)
{
    return ip.template StaticCast<U>();
}

template <typename U, typename T, typename Traits>
edt::IntrusivePtr<U, Traits> dynamic_pointer_cast(const edt::IntrusivePtr<T, Traits>& ip)
{
    return ip.template DynamicCast<U>();
}
}  // namespace std