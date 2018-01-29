#pragma once

namespace edt
{
	template<typename T, typename RefCounter>
	class IntrusivePtr
	{
	public:
		template<typename... Args>
		static IntrusivePtr MakeInstance(Args&&... args)
		{
			return IntrusivePtr(
				new T(std::forward<Args>(args)...));
		}

        IntrusivePtr(nullptr_t) :
            m_p(nullptr)
        {
        }

		explicit IntrusivePtr(T* ptr = nullptr) :
			m_p(ptr)
		{
			AddRef();
		}

		template<typename U, typename Enable =
			std::enable_if_t<std::is_convertible_v<U, T>>>
		IntrusivePtr(const IntrusivePtr<U, RefCounter>& ref) :
			m_p(ref.Get())
		{
			AddRef();
		}

		IntrusivePtr(const IntrusivePtr& ref) :
			m_p(ref.m_p)
		{
			AddRef();
		}

		IntrusivePtr(IntrusivePtr&& that)
		{
			MoveFrom(that);
		}

		~IntrusivePtr()
		{
			ReleaseRef();
		}

		T* Get() const
		{
			return m_p;
		}

		IntrusivePtr& operator=(const IntrusivePtr& ref)
		{
			if (this != &that &&
				m_p != that.m_p)
			{
				ReleaseRef();
				m_p = ref.m_p;
				AddRef();
			}

			return *this;
		}

		IntrusivePtr& operator=(IntrusivePtr&& that)
		{
			if (this != &that &&
				m_p != that.m_p)
			{
				ReleaseRef();
				MoveFrom(that);
			}

			return *this;
		}

		bool operator==(const IntrusivePtr& that) const
		{
			return m_p == that.m_p;
		}

		bool operator!=(const IntrusivePtr& that) const
		{
			return m_p != that.m_p;
		}

        bool operator==(nullptr_t) const {
            return m_p == nullptr;
        }

        bool operator!=(nullptr_t) const {
            return m_p != nullptr;
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

        template<typename U,
            typename Enable = std::enable_if_t<std::is_base_of_v<U, T>>>
        operator IntrusivePtr<U, RefCounter>() const {
            return IntrusivePtr<U, RefCounter>(m_p);
        }

	protected:
		void MoveFrom(IntrusivePtr& ref)
		{
			m_p = ref.m_p;
			ref.m_p = nullptr;
		}

		void AddRef()
		{
			if (m_p != nullptr)
			{
				RefCounter::AddReference(m_p);
			}
		}

		void ReleaseRef()
		{
			if (m_p != nullptr &&
				RefCounter::ReleaseReference(m_p) <= 0)
			{
				delete m_p;
				m_p = nullptr;
			}
		}

		T* m_p;
	};

    template<typename To, typename From, typename RefCounter>
    IntrusivePtr<To, RefCounter> StaticPointerCast(const IntrusivePtr<From, RefCounter>& from) {
        return IntrusivePtr<To, RefCounter>(static_cast<To*>(from.Get()));
    }
}