#pragma once

#include <cassert>
#include <iterator>

namespace edt
{
	namespace array_view_details
	{
		template<typename T>
		size_t PointerToAddress(T* pointer)
		{
			return reinterpret_cast<size_t>(pointer);
		}

		template<typename T>
		T* AddressToPointer(size_t address)
		{
			return reinterpret_cast<T*>(address);
		}

		template<bool direct>
		size_t IncrementAddress(size_t address, size_t stride)
		{
			return address + stride;
		}

		template<>
		size_t IncrementAddress<false>(size_t address, size_t stride)
		{
			assert(address > stride);
			return address - stride;
		}

		template<bool direct>
		size_t DecrementAddress(size_t address, size_t stride)
		{
			return IncrementAddress<!direct>(address, stride);
		}

		template<bool direct>
		size_t AdvanceAddress(size_t address, size_t stride, size_t count)
		{
			return IncrementAddress<direct>(address, stride * count);
		}
	}

	template<typename T, bool direct>
	class Iterator
	{
	public:
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = int;

		Iterator(size_t address, size_t stride) :
			m_address(address),
			m_stride(stride)
		{}
		Iterator(const Iterator&) = default;

		Iterator& operator=(const Iterator&) = default;

		// Pre-increment
		Iterator& operator++()
		{
			Increment();
			return *this;
		}

		// Pre-decrement
		Iterator& operator--()
		{
			Decrement();
			return *this;
		}

		// Postfix increment
		Iterator operator++(int)
		{
			Iterator tmp(*this);
			operator++();
			return tmp;
		}

		// Postfix decrement
		Iterator operator--(int)
		{
			Iterator tmp(*this);
			operator--();
			return tmp;
		}

		bool operator==(const Iterator& another) const
		{
			return
				m_address == another.m_address &&
				m_stride == another.m_stride;
		}

		bool operator!=(const Iterator& another) const
		{
			return !(*this == another);
		}

		reference operator*()
		{
			return *array_view_details::AddressToPointer<T>(m_address);
		}

		pointer operator->()
		{
			return *array_view_details::AddressToPointer<T>(m_address);
		}

	protected:
		void Increment()
		{
			m_address = array_view_details::IncrementAddress<direct>(m_address, m_stride);
		}

		void Decrement()
		{
			m_address = array_view_details::DecrementAddress<direct>(m_address, m_stride);
		}

	private:
		size_t m_address;
		size_t m_stride;
	};

	template<typename T>
	class ArrayView
	{
	public:
		using iterator = Iterator<T, true>;
		using const_iterator = Iterator<const T, true>;
		using reverse_iterator = Iterator<T, false>;
		using const_reverse_iterator = Iterator<const T, false>;

		ArrayView(T* ptr = nullptr, size_t size = 0, size_t stride = sizeof(T)) :
			m_address(array_view_details::PointerToAddress(ptr)),
			m_size(size),
			m_stride(stride)
		{
			assert(stride >= sizeof(T));
			assert(size == 0 || ptr != nullptr);
		}

		decltype(auto) begin() { return begin<T, true>(); }
		decltype(auto) end() { return end<T, true>(); }

		decltype(auto) begin() const { return begin<const T, true>(); }
		decltype(auto) end() const { return end<const T, true>(); }

		decltype(auto) rbegin() { return begin<T, false>(); }
		decltype(auto) rend() { return end<T, false>(); }

		decltype(auto) rbegin() const { return begin<const T, false>(); }
		decltype(auto) rend() const { return end<const T, false>(); }

	private:
		template<typename U, bool direct>
		decltype(auto) begin()
		{
			return Iterator<U, direct>(BeginAddress<direct>(), m_stride);
		}

		template<typename U, bool direct>
		decltype(auto) end()
		{
			return Iterator<U, direct>(EndAddress<direct>(), m_stride);
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
}