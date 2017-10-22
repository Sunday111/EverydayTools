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

		explicit IntrusivePtr(T* ptr = nullptr) :
			m_p(ptr)
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
			MoveFrom(that.m_p);
		}

		~IntrusivePtr()
		{
			ReleaseRef();
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

	protected:
		void MoveFrom(IntrusivePtr& ref)
		{
			m_p = ref.m_p;
			ref.m_p = nullptr;
		}

		T* m_p;

	private:
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
	};
}