#pragma once

#include <cassert>
#include <iterator>

namespace edt
{
	namespace array_view_details
	{
		template<bool direct, typename T>
		inline T* AdvancePointer(T* pointer, size_t count, size_t stride = sizeof(T)) noexcept
		{
			assert(stride >= sizeof(T));

			if constexpr (direct)
			{
				return reinterpret_cast<T*>(
					reinterpret_cast<size_t>(pointer) + count * stride);
			}
			else
			{
				assert(reinterpret_cast<size_t>(pointer) >= count * stride);

				return reinterpret_cast<T*>(
					reinterpret_cast<size_t>(pointer) + count * stride);
			}
		}
	}

	template<
		typename T, bool direct,
		template<typename, bool> typename Final>
	class BaseRandomAccessIterator
	{
	public:
		using TFinal = Final<T, direct>;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = int;

		TFinal& operator++() noexcept
		{
			auto& _this = CastThis();
			_this.Increment();
			return _this;
		}

		TFinal& operator--() noexcept
		{
			auto& _this = CastThis();
			_this.Decrement();
			return _this;
		}

		TFinal operator++(int) noexcept
		{
			TFinal tmp(*this);
			operator++();
			return tmp;
		}

		TFinal operator--(int) noexcept
		{
			TFinal tmp(*this);
			operator--();
			return tmp;
		}

		bool operator==(const TFinal& another) const noexcept
		{
			return CastThis().TheSame(another);
		}

		bool operator!=(const TFinal& another) const noexcept
		{
			return !(*this == another);
		}

		reference operator*() const noexcept
		{
			return *CastThis().GetData();
		}

		pointer operator->() const noexcept
		{
			return CastThis().GetData();
		}

	private:
		TFinal& CastThis() noexcept { return *static_cast<TFinal*>(this); }
		const TFinal& CastThis() const noexcept { return *static_cast<const TFinal*>(this); }
	};

	template<typename T, bool direct>
	class SparseRandomAccessIterator :
		public BaseRandomAccessIterator<T, direct, SparseRandomAccessIterator>
	{
	public:
		SparseRandomAccessIterator(T* ptr, size_t stride) noexcept :
			m_p(ptr),
			m_stride(stride)
		{}

	protected:
		void Increment() noexcept
		{
			m_p = array_view_details::AdvancePointer<true>(m_p, 1, m_stride);
		}

		void Decrement() noexcept
		{
			m_p = array_view_details::AdvancePointer<false>(m_p, 1, m_stride);
		}

		pointer GetData() const noexcept
		{
			return m_p;
		}

		bool TheSame(const SparseRandomAccessIterator& another) const noexcept
		{
			return
				m_p == another.m_p &&
				m_stride == another.m_stride;
		}

	private:
		friend class BaseRandomAccessIterator<T, direct, ::edt::SparseRandomAccessIterator>;

		T* m_p;
		size_t m_stride;
	};

	template<typename T, bool direct>
	class DenseRandomAccessIterator :
		public BaseRandomAccessIterator<T, direct, DenseRandomAccessIterator>
	{
	public:
		DenseRandomAccessIterator(T* ptr) noexcept :
			m_p(ptr)
		{}

	protected:
		void Increment() noexcept
		{
			m_p = array_view_details::IncrementPointer<direct>(m_p);
		}

		void Decrement() noexcept
		{
			m_p = array_view_details::DecrementPointer<direct>(m_p);
		}

		pointer GetData() const noexcept
		{
			return m_p;
		}

		bool TheSame(const DenseRandomAccessIterator& another) const noexcept
		{
			return m_p == another.m_p;
		}

	private:
		friend class BaseRandomAccessIterator<T, direct, ::edt::DenseRandomAccessIterator>;

	private:
		T* m_p;
	};

	template<typename T, template<typename T> typename Final>
	class ArrayView
	{
		using TFinal = Final<T>;

	public:
		decltype(auto) begin() const noexcept
		{
			return Cast().Begin<true>();
		}

		decltype(auto) end() const noexcept
		{
			return Cast().End<true>();
		}

		decltype(auto) rbegin() const noexcept
		{
			return Cast().Begin<false>();
		}

		decltype(auto) rend() const noexcept
		{
			return Cast().End<false>();
		}

	private:
		TFinal& Cast() noexcept
		{
			return *static_cast<TFinal*>(this);
		}

		const TFinal& Cast() const noexcept
		{
			return *static_cast<const TFinal*>(this);
		}
	};

	template<typename T>
	class SparseArrayView :
		public ArrayView<T, SparseArrayView>
	{
	public:
		template<bool direct>
		using TIterator = SparseRandomAccessIterator<T, direct>;
		using iterator = TIterator<true>;
		using const_iterator = iterator;
		using reverse_iterator = TIterator<false>;
		using const_reverse_iterator = reverse_iterator;

		template<typename U, typename Enable =
			std::enable_if_t<
				(std::is_same_v<std::decay_t<T>, std::decay_t<U>> ||
				std::is_base_of_v<T, U>)
				&& std::is_convertible_v<U, T>
			>>
		SparseArrayView(const SparseArrayView<U>& another) noexcept :
			SparseArrayView(another.GetData(), another.GetSize(), another.GetStride())
		{}

		SparseArrayView(T* ptr = nullptr, size_t size = 0, size_t stride = sizeof(T)) noexcept :
			m_p(ptr),
			m_size(size),
			m_stride(stride)
		{
			assert(stride >= sizeof(T));
			assert(size == 0 || ptr != nullptr);
		}

		size_t GetSize() const noexcept
		{
			return m_size;
		}

		size_t GetStride() const noexcept
		{
			return m_stride;
		}

		T* GetData() const noexcept
		{
			return m_p;
		}

		template<typename MemberPtr,
			class = std::enable_if_t<std::is_member_object_pointer_v<MemberPtr>>>
		decltype(auto) MakeMemberView(MemberPtr member) const noexcept
		{
			using namespace array_view_details;
			using CleanMemberType = decltype(GetMemberValue(*GetData(), member));
			using MemberType = std::remove_reference_t<CleanMemberType>;
			return SparseArrayView<MemberType>(&GetMemberValue(*GetData(), member), GetSize(), GetStride());
		}

	private:
		template<bool direct>
		decltype(auto) Begin() const noexcept
		{
			return TIterator<direct>(BeginPointer<direct>(), m_stride);
		}

		template<bool direct>
		decltype(auto) End() const noexcept
		{
			return TIterator<direct>(EndPointer<direct>(), m_stride);
		}

		template<bool direct>
		T* BeginPointer() const noexcept
		{
			if constexpr (direct)
			{
				return m_p;
			}
			else
			{
				return
					m_size > 0 ?
					array_view_details::AdvancePointer(m_p, m_size - 1, m_stride) :
					m_address;
			}
		}

		template<bool direct>
		T* EndPointer() const noexcept
		{
			return array_view_details::AdvancePointer<direct>(
				BeginPointer<direct>(), m_size, m_stride);
		}

		friend ArrayView<T, ::edt::SparseArrayView>;

		T* m_p;
		size_t m_size;
		size_t m_stride;
	};

	template<typename T>
	class DenseArrayView :
		public ArrayView<T, DenseArrayView>
	{
	public:
		template<bool direct>
		using TIterator = DenseRandomAccessIterator<T, direct>;
		using iterator = TIterator<true>;
		using const_iterator = iterator;
		using reverse_iterator = TIterator<false>;
		using const_reverse_iterator = reverse_iterator;

		/// Constructs dense array view with dense array view instantiated with another T
		/**
			Allows to construct with DenseArrayView instantiated with the same type
			but with another qualifiers (for example from T = 'int' to V = 'const int')

			@param another - array view to construct from
		 */
		template<typename U, typename Enable =
			std::enable_if_t<
				std::is_same_v<std::decay_t<U>, std::decay_t<T>> &&
				std::is_convertible_v<U, T>
			>>
		DenseArrayView(const DenseArrayView<U>& another) noexcept :
			DenseArrayView(another.GetData(), another.GetSize())
		{}

		/// Default constructor and constructor with all parameters
		/**
			@param[in] ptr - pointer to the first element
			@param[in] size - elements count
		 */
		DenseArrayView(T* ptr = nullptr, size_t size = 0) noexcept :
			m_p(ptr),
			m_size(size)
		{
			assert(size == 0 || ptr != nullptr);
		}

		/// The size of view
		/**
			@return viewed elements count
		 */
		size_t GetSize() const noexcept
		{
			return m_size;
		}

		/// Pointer to the first element
		/**
			@return pointer to the first element of viewed collection
		 */
		T* GetData() const noexcept
		{
			return m_p;
		}

		/// Constructs sparse array view to member
		/**
			Constructs sparse array view to some data member of T type
			@param member - pointer to class member
			@return members array view
		 */
		template<typename MemberPtr,
			class = std::enable_if_t<std::is_member_object_pointer<MemberPtr>::value>>
		decltype(auto) MakeMemberView(MemberPtr member) const noexcept
		{
			using namespace array_view_details;
			using CleanMemberType = decltype(GetMemberValue(*GetData(), member));
			using MemberType = std::remove_reference_t<CleanMemberType>;
			return SparseArrayView<MemberType>(&GetMemberValue(*GetData(), member), GetSize(), sizeof(T));
		}

		/// Casts dense array view to sparse array view of base type
		/**
			Casts to sparse array view instantiated with base type of T

			@return sparse view to base type
		 */
		template<typename U,
			class = std::enable_if_t<
				std::is_base_of_v<U, T> &&
				std::is_convertible_v<T, U>>>
		operator SparseArrayView<U>() const noexcept
		{
			// Stride may not change here
			return SparseArrayView<U>(m_p, m_size, sizeof(T));
		}

	private:
		template<bool direct>
		decltype(auto) Begin() const noexcept
		{
			return TIterator<direct>(BeginPointer<direct>());
		}

		template<bool direct>
		decltype(auto) End() const noexcept
		{
			return TIterator<direct>(EndPointer<direct>());
		}

		template<bool direct>
		T* BeginPointer() const noexcept
		{
			return m_p;
		}

		template<>
		T* BeginPointer<false>() const noexcept
		{
			return m_size > 0 ?
				m_p + (m_size - 1) :
				m_p;
		}

		template<bool direct>
		T* EndPointer() const noexcept
		{
			return array_view_details::AdvancePointer<direct>(
				BeginPointer<direct>(), static_cast<int>(m_size));
		}

		friend ArrayView<T, ::edt::DenseArrayView>;

		T* m_p;
		size_t m_size;
	};

	template<typename T, size_t size>
	DenseArrayView<T> MakeArrayView(T(&arr)[size]) noexcept
	{
		return DenseArrayView<T>(arr, size);
	}

	template<typename T, typename Member, size_t size>
	SparseArrayView<Member> MakeArrayView(T(&arr)[size], Member T::* member) noexcept
	{
		return SparseArrayView<Member>(&(*arr.*member), size, sizeof(T));
	}
}