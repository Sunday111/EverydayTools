#pragma once

namespace edt
{
    template<class T, typename Traits>
    class IntrusivePtr
    {
    public:
        IntrusivePtr() :
            m_p(nullptr)
        {}

        IntrusivePtr(std::nullptr_t) :
            m_p(nullptr)
        {}

        IntrusivePtr(T* p) :
            m_p(p)
        {
            AddReference();
        }

        explicit IntrusivePtr(IntrusivePtr&& ptr) :
            m_p(ptr.m_p)
        {
            ptr.m_p = nullptr;
        }

        explicit IntrusivePtr(const IntrusivePtr& ptr) :
            m_p(ptr.m_p)
        {
            AddReference();
        }

        template<typename U>
        IntrusivePtr(const IntrusivePtr<U, Traits>& ip,
            typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0) :
            m_p(ip.m_p)
        {
            AddReference();
        }

        template<typename U>
        IntrusivePtr(IntrusivePtr<U, Traits>&& ip,
            typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0) :
            m_p(ip.m_p)
        {
            ip.m_p = nullptr;
        }

        ~IntrusivePtr() {
            ReleaseReference();
        }

        void reset() {
            ReleaseReference(m_p);
            m_p = nullptr;
        }

        IntrusivePtr& swap(IntrusivePtr& p) {
            auto tmp = m_p;
            m_p = p.m_p;
            p.m_p = tmp;
            return *this;
        }

        T* Get() const {
            return m_p;
        }

        template<typename U>
        IntrusivePtr<U, Traits> StaticCast() const {
            typename IntrusivePtr<U, Traits>::StaticCastTag tag;
            return IntrusivePtr<U, Traits>(*this, tag);
        }

        template<typename U>
        IntrusivePtr<U, Traits> DynamicCast() const {
            typename IntrusivePtr<U, Traits>::DynamicCastTag tag;
            return IntrusivePtr<U, Traits>(*this, tag);
        }

        IntrusivePtr& operator=(const IntrusivePtr& ptr) {
            if (this != &ptr) {
                ReleaseReference();
                m_p = ptr.m_p;
                AddReference();
            }
            return *this;
        }

        IntrusivePtr& operator=(IntrusivePtr&& ptr) {
            if (this != &ptr) {
                ReleaseReference();
                m_p = ptr.m_p;
                ptr.m_p = nullptr;
            }
            return *this;
        }

        template<typename U>
        IntrusivePtr& operator=(const IntrusivePtr<U, Traits>& ptr) {
            if (this != &ptr) {
                ReleaseReference(m_p);
                m_p = ptr.m_p;
                AddReference(m_p);
            }
            return *this;
        }

        template<typename U>
        IntrusivePtr& operator=(IntrusivePtr<U, Traits>&& ptr) {
            if (this != &ptr) {
                ReleaseReference(m_p);
                m_p = ptr.m_p;
                ptr.m_p = nullptr;
            }

            return *this;
        }

        bool operator==(std::nullptr_t) const {
            return m_p == nullptr;
        }

        bool operator!=(std::nullptr_t) const {
            return m_p != nullptr;
        }

        template<typename U>
        bool operator==(const IntrusivePtr<U, Traits>& p) const {
            return m_p == p.m_p;
        }

        template<typename U>
        bool operator!=(const IntrusivePtr<U, Traits>& p) {
            return m_p != p.m_p;
        }

        template<typename U>
        bool operator<(const IntrusivePtr<U, Traits>& p) const {
            return m_p < p.m_p;
        }

        T* operator->() const {
            assert(m_p != nullptr);
            return m_p;
        }

        T& operator*() const {
            assert(m_p != nullptr);
            return *m_p;
        }

        template<typename... Args>
        static IntrusivePtr MakeInstance(Args&&... args) {
            return IntrusivePtr(new T(std::forward<Args>(args)...));
        }

    protected:
        inline void AddReference() {
            if (m_p != nullptr) {
                Traits::AddReference(m_p);
            }
        }

        inline void ReleaseReference() {
            if (m_p != nullptr) {
                Traits::ReleaseReference(m_p);
            }
        }

    private:
        template<typename, typename>
        friend class IntrusivePtr;

        struct StaticCastTag {};
        struct DynamicCastTag {};

        template<typename U>
        IntrusivePtr(const IntrusivePtr<U, Traits>& ip, StaticCastTag) :
            m_p(static_cast<T*>(ip.m_p))
        {
            AddReference();
        }

        template<typename U>
        IntrusivePtr(const IntrusivePtr<U, Traits>& ip, DynamicCastTag) :
            m_p(dynamic_cast<T*>(ip.m_p))
        {
            AddReference();
        }

        T* m_p;
    };
}

namespace std {
    template<typename U, typename T, typename Traits>
    edt::IntrusivePtr<U, typename Traits> static_pointer_cast(const edt::IntrusivePtr<T, typename Traits>& ip) {
        return ip.template StaticCast<U>();
    }

    template<typename U, typename T, typename Traits>
    edt::IntrusivePtr<U, Traits> dynamic_pointer_cast(const edt::IntrusivePtr<T, Traits>& ip) {
        return ip.template DynamicCast<U>();
    }
}