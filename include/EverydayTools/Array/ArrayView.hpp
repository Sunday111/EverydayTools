#pragma once

#include <cassert>
#include <iterator>
#include <type_traits>

namespace edt::array_view_details
{
template <bool direct, typename T>
[[nodiscard]] inline T* AdvancePointer(T* pointer, size_t count) noexcept
{
    if constexpr (direct)
    {
        return pointer + count;
    }
    else
    {
        return pointer - count;
    }
}

template <bool direct, typename T>
[[nodiscard]] inline T* AdvancePointer(T* pointer, size_t count, size_t stride) noexcept
{
    assert(stride >= sizeof(T));

    if constexpr (direct)
    {
        return reinterpret_cast<T*>(reinterpret_cast<size_t>(pointer) + count * stride);  // NOLINT
    }
    else
    {
        assert(reinterpret_cast<size_t>(pointer) >= count * stride);  // NOLINT

        return reinterpret_cast<T*>(reinterpret_cast<size_t>(pointer) - count * stride);  // NOLINT
    }
}

template <typename T, typename TMember>
[[nodiscard]] constexpr inline decltype(auto) GetMemberValue(T& t, TMember member) noexcept
{
    return (t.*member);
}
}  // namespace edt::array_view_details

namespace edt
{
template <typename T, bool direct, template <typename, bool> typename Final>
class RandomAccessIterator
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

    const TFinal operator++(int) noexcept
    {
        TFinal tmp(CastThis());
        operator++();
        return tmp;
    }

    const TFinal operator--(int) noexcept
    {
        TFinal tmp(CastThis());
        operator--();
        return tmp;
    }

    TFinal& operator+=(int val) noexcept
    {
        auto& _this = CastThis();
        _this.Advance(val);
        return _this;
    }

    TFinal operator+(int val) const noexcept
    {
        TFinal tmp(CastThis());
        tmp += val;
        return tmp;
    }

    [[nodiscard]] friend bool operator<(const TFinal& a, const TFinal b) noexcept { return a.GetData() < b.GetData(); }

    [[nodiscard]] reference operator*() const noexcept { return *CastThis().GetData(); }

    [[nodiscard]] pointer operator->() const noexcept { return CastThis().GetData(); }

private:
    TFinal& CastThis() noexcept { return *static_cast<TFinal*>(this); }
    const TFinal& CastThis() const noexcept { return *static_cast<const TFinal*>(this); }
};

template <typename T, bool direct>
class SparseRandomAccessIterator : public RandomAccessIterator<T, direct, SparseRandomAccessIterator>
{
public:
    constexpr SparseRandomAccessIterator(T* ptr, size_t stride) noexcept : m_p(ptr), m_stride(stride) {}

    void Advance(int n) { m_p = array_view_details::AdvancePointer<direct>(m_p, n, m_stride); }

    void Increment() noexcept { m_p = array_view_details::AdvancePointer<direct>(m_p, 1, m_stride); }

    void Decrement() noexcept { m_p = array_view_details::AdvancePointer<!direct>(m_p, 1, m_stride); }

    [[nodiscard]] bool TheSame(const SparseRandomAccessIterator& another) const noexcept
    {
        return m_p == another.m_p && m_stride == another.m_stride;
    }

    [[nodiscard]] T* GetData() const noexcept { return m_p; }

    [[nodiscard]] bool operator==(const SparseRandomAccessIterator& another) const noexcept { return TheSame(another); }

    [[nodiscard]] bool operator!=(const SparseRandomAccessIterator& another) const noexcept
    {
        return !(*this == another);
    }

private:
    T* m_p;
    size_t m_stride;
};

template <typename T, bool direct>
class DenseRandomAccessIterator : public RandomAccessIterator<T, direct, DenseRandomAccessIterator>
{
public:
    explicit constexpr DenseRandomAccessIterator(T* ptr) noexcept : m_p(ptr) {}

    void Advance(int n) { m_p = array_view_details::AdvancePointer<direct>(m_p, n); }

    void Increment() noexcept { m_p = array_view_details::AdvancePointer<direct>(m_p, 1); }

    void Decrement() noexcept { m_p = array_view_details::AdvancePointer<!direct>(m_p, 1); }

    [[nodiscard]] constexpr bool TheSame(const DenseRandomAccessIterator& another) const noexcept
    {
        return m_p == another.m_p;
    }

    [[nodiscard]] constexpr T* GetData() const noexcept { return m_p; }

    [[nodiscard]] bool operator==(const DenseRandomAccessIterator& another) const noexcept { return TheSame(another); }

    [[nodiscard]] bool operator!=(const DenseRandomAccessIterator& another) const noexcept
    {
        return !(*this == another);
    }

private:
    T* m_p;
};

template <typename T, template <typename T_> typename Final>
class ArrayView
{
    using TFinal = Final<T>;

public:
    [[nodiscard]] decltype(auto) begin() const noexcept { return Cast().template Begin<true>(); }

    [[nodiscard]] decltype(auto) end() const noexcept { return Cast().template End<true>(); }

    [[nodiscard]] decltype(auto) rbegin() const noexcept { return Cast().template Begin<false>(); }

    [[nodiscard]] decltype(auto) rend() const noexcept { return Cast().template End<false>(); }

private:
    [[nodiscard]] TFinal& Cast() noexcept { return *static_cast<TFinal*>(this); }

    [[nodiscard]] const TFinal& Cast() const noexcept { return *static_cast<const TFinal*>(this); }
};

template <typename T>
class DenseArrayView;

template <typename T>
class SparseArrayView : public ArrayView<T, edt::SparseArrayView>
{
    // Make base class able to use protected methods
    friend ArrayView<T, edt::SparseArrayView>;

public:
    template <bool direct>
    using TIterator = SparseRandomAccessIterator<T, direct>;
    using iterator = TIterator<true>;
    using const_iterator = iterator;
    using reverse_iterator = TIterator<false>;
    using const_reverse_iterator = reverse_iterator;

    /// Template copy constructor
    /**
        Constructs array view using reference to the object of the same type
        or using regerence to array view instantiated with derived type
        @param another - object to copy
     */
    template <
        typename U,
        typename Enable = std::enable_if_t<
            (std::is_same_v<std::decay_t<T>, std::decay_t<U>> || std::is_base_of_v<T, U>) &&
            std::is_convertible_v<U, T>>>
    constexpr SparseArrayView(const SparseArrayView<U>& another) noexcept
        : SparseArrayView(another.GetData(), another.GetSize(), another.GetStride())
    {
    }

    /// Constructor
    /**
        Constructor by parameters and default constructor
        @param ptr - pointer to first object
        @param size - objects count
        @param stride - distance between objects
     */
    constexpr SparseArrayView(T* ptr = nullptr, size_t size = 0, size_t stride = sizeof(T)) noexcept
        : m_p(ptr),
          m_size(size),
          m_stride(stride)
    {
        assert(stride >= sizeof(T));
        assert(size == 0 || ptr != nullptr);
    }

    /// Returns objects count
    /**
        @return viewed objects count
     */
    [[nodiscard]] constexpr size_t GetSize() const noexcept { return m_size; }

    /// Returns stride
    /**
        @return stride between objects in view
     */
    [[nodiscard]] constexpr size_t GetStride() const noexcept { return m_stride; }

    /// Returns internal pointer
    /**
        @return pointer to the first object
     */
    [[nodiscard]] constexpr T* GetData() const noexcept { return m_p; }

    /// Constructs sparse array view to member
    /**
        Constructs sparse array view to some data member of T type
        @param member - pointer to class member
        @return members array view
     */
    template <typename MemberPtr, class = std::enable_if_t<std::is_member_object_pointer_v<MemberPtr>>>
    [[nodiscard]] constexpr decltype(auto) MakeMemberView(MemberPtr member) const noexcept
    {
        using namespace array_view_details;
        using CleanMemberType = decltype(GetMemberValue(*GetData(), member));
        using MemberType = std::remove_reference_t<CleanMemberType>;
        return SparseArrayView<MemberType>(&GetMemberValue(*GetData(), member), GetSize(), GetStride());
    }

    /// Assigns view to some value
    /**
        Sets every value of viewed collection to passed value
        @param value - value to use
     */
    void Assign(const T& value) const
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            operator[](i) = value;
        }
    }

    /// Splits array view
    /*
        Makes new array view that represents some subset of originale one
        @param startIndex - first element of resulting subset
        @param count - the number of elements in result
     */
    [[nodiscard]] constexpr SparseArrayView<T> Split(size_t startIndex, size_t count) const
    {
        assert(startIndex + count <= m_size);
        return SparseArrayView<T>(array_view_details::AdvancePointer<true>(m_p, startIndex, m_stride), count, m_stride);
    }

    [[nodiscard]] T& operator[](size_t index) const noexcept
    {
        assert(index < m_size);
        return *array_view_details::AdvancePointer<true>(m_p, index, m_stride);
    }

    SparseArrayView<T>& operator=(const DenseArrayView<T>& dense)
    {
        m_p = dense.GetData();
        m_size = dense.GetSize();
        m_stride = sizeof(T);
        return *this;
    }

protected:
    template <bool direct>
    [[nodiscard]] decltype(auto) Begin() const noexcept
    {
        return TIterator<direct>(BeginPointer<direct>(), m_stride);
    }

    template <bool direct>
    [[nodiscard]] decltype(auto) End() const noexcept
    {
        return TIterator<direct>(EndPointer<direct>(), m_stride);
    }

    template <bool direct>
    [[nodiscard]] T* BeginPointer() const noexcept
    {
        if constexpr (direct)
        {
            return m_p;
        }
        else
        {
            return m_size > 0 ? array_view_details::AdvancePointer<true>(m_p, m_size - 1, m_stride) : m_p;
        }
    }

    template <bool direct>
    [[nodiscard]] T* EndPointer() const noexcept
    {
        return array_view_details::AdvancePointer<direct>(BeginPointer<direct>(), m_size, m_stride);
    }

private:
    T* m_p;
    size_t m_size;
    size_t m_stride;
};

template <typename T>
class DenseArrayView : public ArrayView<T, edt::DenseArrayView>
{
    // Make base class able to use protected methods
    friend ArrayView<T, edt::DenseArrayView>;

public:
    template <bool directIteration>
    using TIterator = DenseRandomAccessIterator<T, directIteration>;
    using iterator = TIterator<true>;
    using const_iterator = iterator;
    using reverse_iterator = TIterator<false>;
    using const_reverse_iterator = reverse_iterator;
    using value_type = T;

    /// Constructs dense array view with dense array view instantiated with
    /// another T
    /**
        Allows to construct with DenseArrayView instantiated with the same type
        but with another qualifiers (for example from T = 'int' to V = 'const
       int')

        @param another - array view to construct from
     */
    template <
        typename U,
        typename Enable =
            std::enable_if_t<std::is_same_v<std::decay_t<U>, std::decay_t<T>> && std::is_convertible_v<U, T>>>
    constexpr DenseArrayView(const DenseArrayView<U>& another) noexcept
        : DenseArrayView(another.GetData(), another.GetSize())
    {
    }

    /// Default constructor and constructor with all parameters
    /**
        @param[in] ptr - pointer to the first element
        @param[in] size - elements count
     */
    constexpr DenseArrayView(T* ptr = nullptr, size_t size = 0) noexcept : m_p(ptr), m_size(size)
    {
        assert(size == 0 || ptr != nullptr);
    }

    /// The size of view
    /**
        @return viewed elements count
     */
    [[nodiscard]] constexpr size_t GetSize() const noexcept { return m_size; }

    /// Pointer to the first element
    /**
        @return pointer to the first element of viewed collection
     */
    [[nodiscard]] constexpr T* GetData() const noexcept { return m_p; }

    /// Constructs sparse array view to member
    /**
        Constructs sparse array view to some data member of T type
        @param member - pointer to class member
        @return members array view
     */
    template <typename MemberPtr, class = std::enable_if_t<std::is_member_object_pointer<MemberPtr>::value>>
    [[nodiscard]] decltype(auto) MakeMemberView(MemberPtr member) const noexcept
    {
        using namespace array_view_details;
        using CleanMemberType = decltype(GetMemberValue(*GetData(), member));
        using MemberType = std::remove_reference_t<CleanMemberType>;
        return SparseArrayView<MemberType>(&GetMemberValue(*GetData(), member), GetSize(), sizeof(T));
    }

    /// Splits array view
    /*
        Makes new array view that represents some subset of originale one
        @param startIndex - first element of resulting subset
        @param count - the number of elements in result
     */
    [[nodiscard]] SparseArrayView<T> Split(size_t startIndex, size_t count) const noexcept
    {
        assert(startIndex + count <= m_size);
        return SparseArrayView<T>(array_view_details::AdvancePointer<true>(m_p, startIndex), count);
    }

    /// Casts dense array view to sparse array view of base type
    /**
        Casts to sparse array view instantiated with base type of T

        @return sparse view to base type
     */
    template <typename U, class = std::enable_if_t<std::is_convertible_v<T, U>>>
    [[nodiscard]] constexpr operator SparseArrayView<U>() const noexcept
    {
        // Stride may not change here
        return SparseArrayView<U>(m_p, m_size, sizeof(T));
    }

    [[nodiscard]] T& operator[](size_t index) const noexcept
    {
        assert(index < m_size);
        return *array_view_details::AdvancePointer<true>(m_p, index);
    }

protected:
    template <bool direct>
    [[nodiscard]] decltype(auto) Begin() const noexcept
    {
        return TIterator<direct>(BeginPointer<direct>());
    }

    template <bool direct>
    [[nodiscard]] decltype(auto) End() const noexcept
    {
        return TIterator<direct>(EndPointer<direct>());
    }

    template <bool direct>
    [[nodiscard]] T* BeginPointer() const noexcept
    {
        if constexpr (direct)
        {
            return m_p;
        }
        else
        {
            return m_size > 0 ? array_view_details::AdvancePointer<true>(m_p, m_size - 1) : nullptr;
        }
    }

    template <bool direct>
    [[nodiscard]] T* EndPointer() const noexcept
    {
        return array_view_details::AdvancePointer<direct>(BeginPointer<direct>(), m_size);
    }

private:
    T* m_p;
    size_t m_size;
};

template <typename T, size_t size>
[[nodiscard]] inline constexpr DenseArrayView<T> MakeArrayView(T (&arr)[size]) noexcept
{
    return DenseArrayView<T>(arr, size);
}

template <typename T, typename Member, size_t size>
[[nodiscard]] inline constexpr SparseArrayView<Member> MakeArrayView(T (&arr)[size], Member T::* member) noexcept
{
    return SparseArrayView<Member>(&(*arr.*member), size, sizeof(T));
}
}  // namespace edt
