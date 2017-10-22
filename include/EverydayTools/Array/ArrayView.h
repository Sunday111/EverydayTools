#pragma once

#include <cassert>
#include <iterator>

namespace edt
{
	namespace array_view_details
	{
		template<typename T>
		inline size_t PointerToAddress(T* pointer)
		{
			return reinterpret_cast<size_t>(pointer);
		}

		template<typename T>
		inline T* AddressToPointer(size_t address)
		{
			return reinterpret_cast<T*>(address);
		}

		template<bool direct>
		inline size_t AdvanceAddress(size_t address, size_t stride, size_t elements)
		{
			return address + stride * elements;
		}

		template<>
		inline size_t AdvanceAddress<false>(size_t address, size_t stride, size_t elements)
		{
			assert(address >= stride * elements);
			return address - stride * elements;
		}

		template<bool direct>
		inline size_t IncrementAddress(size_t address, size_t stride)
		{
			return address + stride;
		}

		template<>
		inline size_t IncrementAddress<false>(size_t address, size_t stride)
		{
			assert(address > stride);
			return address - stride;
		}

		template<bool direct>
		inline size_t DecrementAddress(size_t address, size_t stride)
		{
			return IncrementAddress<!direct>(address, stride);
		}

		template<typename T, typename TMember>
		inline decltype(auto) GetMemberValue(T& t, TMember member)
		{
			return (t.*member);
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

		TFinal& operator++()
		{
			auto& _this = CastThis();
			_this.Increment();
			return _this;
		}

		TFinal& operator--()
		{
			auto& _this = CastThis();
			_this.Decrement();
			return _this;
		}

		TFinal operator++(int)
		{
			TFinal tmp(*this);
			operator++();
			return tmp;
		}

		TFinal operator--(int)
		{
			TFinal tmp(*this);
			operator--();
			return tmp;
		}

		bool operator==(const TFinal& another) const
		{
			return CastThis().TheSame(another);
		}

		bool operator!=(const TFinal& another) const
		{
			return !(*this == another);
		}

		reference operator*() const
		{
			return *CastThis().GetData();
		}

		pointer operator->() const
		{
			return CastThis().GetData();
		}

	private:
		TFinal& CastThis() { return *static_cast<TFinal*>(this); }
		const TFinal& CastThis() const { return *static_cast<const TFinal*>(this); }
	};

	template<typename T, bool direct>
	class SparseRandomAccessIterator :
		public BaseRandomAccessIterator<T, direct, SparseRandomAccessIterator>
	{
	public:
		SparseRandomAccessIterator(size_t address, size_t stride) :
			m_address(address),
			m_stride(stride)
		{}

	protected:
		void Increment()
		{
			m_address = array_view_details::IncrementAddress<direct>(m_address, m_stride);
		}

		void Decrement()
		{
			m_address = array_view_details::DecrementAddress<direct>(m_address, m_stride);
		}

		pointer GetData() const
		{
			return array_view_details::AddressToPointer<T>(m_address);
		}

		bool TheSame(const SparseRandomAccessIterator& another) const
		{
			return
				m_address == another.m_address &&
				m_stride == another.m_stride;
		}

	private:
		friend class BaseRandomAccessIterator<T, direct, ::edt::SparseRandomAccessIterator>;

		size_t m_address;
		size_t m_stride;
	};

	template<typename T>
	class ArrayView
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
				std::is_same<std::decay_t<T>, std::decay_t<U>>::value &&
				(std::is_const<T>::value || !(std::is_const<U>::value))
			>>
		ArrayView(const ArrayView<U>& another) :
			ArrayView(another.GetData(), another.GetSize(), another.GetStride())
		{}

		ArrayView(T* ptr = nullptr, size_t size = 0, size_t stride = sizeof(T)) :
			m_address(array_view_details::PointerToAddress(ptr)),
			m_size(size),
			m_stride(stride)
		{
			assert(stride >= sizeof(T));
			assert(size == 0 || ptr != nullptr);
		}

		size_t GetSize() const
		{
			return m_size;
		}

		size_t GetStride() const
		{
			return m_stride;
		}

		T* GetData() const
		{
			return array_view_details::AddressToPointer<T>(m_address);
		}

		template<typename MemberPtr,
			class = std::enable_if_t<std::is_member_object_pointer<MemberPtr>::value>>
		decltype(auto) MakeMemberView(MemberPtr member) const
		{
			using namespace array_view_details;
			using CleanMemberType = decltype(GetMemberValue(*GetData(), member));
			using MemberType = std::remove_reference_t<CleanMemberType>;
			return ArrayView<MemberType>(&GetMemberValue(*GetData(), member), GetSize(), GetStride());
		}

		decltype(auto) begin() const
		{
			return begin<true>();
		}

		decltype(auto) end() const
		{
			return end<true>();
		}

		decltype(auto) rbegin() const
		{
			return begin<false>();
		}

		decltype(auto) rend() const
		{
			return end<false>();
		}

	private:
		template<bool direct>
		decltype(auto) begin() const
		{
			return TIterator<direct>(BeginAddress<direct>(), m_stride);
		}

		template<bool direct>
		decltype(auto) end() const
		{
			return TIterator<direct>(EndAddress<direct>(), m_stride);
		}

		template<bool direct>
		size_t BeginAddress() const
		{
			return m_address;
		}

		template<>
		size_t BeginAddress<false>() const
		{
			return m_size > 0 ?
				m_address + (m_size - 1) * m_stride :
				m_address;
		}

		template<bool direct>
		size_t EndAddress() const
		{
			return array_view_details::AdvanceAddress<direct>(
				BeginAddress<direct>(), m_stride, m_size);
		}

		size_t m_address;
		size_t m_size;
		size_t m_stride;
	};

	template<typename T, size_t size>
	ArrayView<T> MakeArrayView(T(&arr)[size])
	{
		return ArrayView<T>(arr, size);
	}

	template<typename T, typename Member, size_t size>
	ArrayView<Member> MakeArrayView(T(&arr)[size], Member T::* member)
	{
		return ArrayView<Member>(&(*arr.*member), size, sizeof(T));
	}
}