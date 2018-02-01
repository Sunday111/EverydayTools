#pragma once

#include <cmath>
#include <type_traits>

#define EDT_IMPLEMENT_CAST_THIS                     \
    using TFinal = Final<T, nRows, nColumns>;       \
    TFinal& CastThis() noexcept {                   \
        return static_cast<TFinal&>(*this);         \
    }                                               \
                                                    \
    const TFinal& CastThis() const noexcept {       \
        return static_cast<const TFinal&>(*this);   \
    }

#ifdef _MSC_VER
    // microsoft compiler produces extra bytes 
    // while using multiple inheritance
    #define _EMPTY_BASES __declspec(empty_bases)
#else
    #define _EMPTY_BASES
#endif

namespace edt::geom::details {

    template<size_t Rows, size_t Columns, size_t N>
    constexpr bool is_nd_vector =
        (Rows == 1 && Columns == N) ||
        (Rows == N && Columns == 1);

    template<size_t Rows, size_t Columns>
    constexpr bool is_vector = Rows == 1 || Columns == 1;

    template
    <
        typename T,
        size_t N
    >
    using Row = T[N];

    template
    <
        typename T,
        size_t nRows,
        size_t nColumns
    >
    using Table = Row<T, nColumns>[nRows];
}

namespace edt::geom::details::data {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class DataMixin
    {
    public:
        T& At(size_t nRow, size_t nColumn) noexcept {
            return data[nRow][nColumn];
        }

        const T& At(size_t nRow, size_t nColumn) const noexcept {
            return data[nRow][nColumn];
        }

        T* GetData() {
            return &data[0][0];
        }

        const T* GetData() const {
            return &data[0][0];
        }

        Table<T, nRows, nColumns> data;
    };
}

namespace edt::geom::details::vector_data_access {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        typename Enable = void
    >
    class VectorDataAccessMixin
    {
    public:
    };

    // Acessors for two dimensional vectors
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class VectorDataAccessMixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_nd_vector<nRows, nColumns, 2>>>
    {
        EDT_IMPLEMENT_CAST_THIS
    public:
        T x() const { return CastThis().GetData()[0]; }
        T y() const { return CastThis().GetData()[1]; }

        T i() const { return CastThis().GetData()[0]; }
        T j() const { return CastThis().GetData()[1]; }

        T& rx() { return CastThis().GetData()[0]; }
        const T& rx() const { return CastThis().GetData()[0]; }
        T& ry() { return CastThis().GetData()[1]; }
        const T& ry() const { return CastThis().GetData()[1]; }

        T& ri() { return CastThis().GetData()[0]; }
        const T& ri() const { return CastThis().GetData()[0]; }
        T& rj() { return CastThis().GetData()[1]; }
        const T& rj() const { return CastThis().GetData()[1]; }
    };

    // Acessors for three dimensional vectors
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class VectorDataAccessMixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_nd_vector<nRows, nColumns, 3>>>
    {
    public:
        T x() const { return CastThis().GetData()[0]; }
        T y() const { return CastThis().GetData()[1]; }
        T z() const { return CastThis().GetData()[2]; }

        T i() const { return CastThis().GetData()[0]; }
        T j() const { return CastThis().GetData()[1]; }
        T k() const { return CastThis().GetData()[2]; }

        T& rx() { return CastThis().GetData()[0]; }
        const T& rx() const { return CastThis().GetData()[0]; }
        T& ry() { return CastThis().GetData()[1]; }
        const T& ry() const { return CastThis().GetData()[1]; }
        T& rz() { return CastThis().GetData()[2]; }
        const T& rz() const { return CastThis().GetData()[2]; }

        T& ri() { return CastThis().GetData()[0]; }
        const T& ri() const { return CastThis().GetData()[0]; }
        T& rj() { return CastThis().GetData()[1]; }
        const T& rj() const { return CastThis().GetData()[1]; }
        T& rk() { return CastThis().GetData()[2]; }
        const T& rk() const { return CastThis().GetData()[2]; }
    };
}

namespace edt::geom::details::vector_rotation {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        typename Enable = void
    >
    class VectorRotationMixin
    {
    };

    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class VectorRotationMixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_nd_vector<nRows, nColumns, 2>>>
    {
        EDT_IMPLEMENT_CAST_THIS
    public:
        TFinal Rotated(T a) const noexcept {
            auto& this_ = CastThis();
            auto cosa = std::cos(a);
            auto sina = std::sin(a);
            TFinal res;
            res.rx() = this_.x() * cosa - this_.y() * sina;
            res.ry() = this_.x() * sina + this_.y() * cosa;
            return res;
        }

        void Rotate(T a) noexcept {
            auto& this_ = CastThis();
            this_ = this_.Rotated(a);
        }
    };
}

namespace edt::geom::details::cast {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        typename Enable = void
    >
    class CastMixin
    {
        EDT_IMPLEMENT_CAST_THIS
    public:
        template<typename U,
            typename Enable = std::enable_if_t<std::is_convertible_v<T, U>>>
        Final<U, nRows, nColumns> Cast() const noexcept {
            Final<U, nRows, nColumns> that_;
            auto& this_ = CastThis();
            for (size_t i = 0; i < nRows; ++i) {
                for (size_t j = 0; j < nColumns; ++j) {
                    that_.At(i, j) = static_cast<U>(this_.At(i, j));
                }
            }
            return that_;
        }
    };
}

namespace edt::geom::details::common {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class CommonMixin
    {
        EDT_IMPLEMENT_CAST_THIS
    public:
        template<typename U>
        TFinal& operator*=(U value) noexcept {
            auto& this_ = CastThis();
            for (size_t i = 0; i < nRows; ++i) {
                for (size_t j = 0; j < nColumns; ++j) {
                    this_.At(i, j) *= value;
                }
            }
            return this_;
        }

        template<typename U>
        TFinal operator*(U value) const noexcept {
            TFinal result(CastThis());
            result *= value;
            return result;
        }

        template<
            typename U,
            template<typename, size_t, size_t> typename Mtx>
        TFinal& operator+=(const Mtx<U, nRows, nColumns>& that_) noexcept {
            auto& this_ = CastThis();
            for (size_t i = 0; i < nRows; ++i) {
                for (size_t j = 0; j < nColumns; ++j) {
                    this_.At(i, j) += that_.At(i, j);
                }
            }
            return this_;
        }
        
        template<
            typename U,
            template<typename, size_t, size_t> typename Mtx>
        TFinal operator+(const Mtx<U, nRows, nColumns>& that_) const noexcept {
            TFinal result(CastThis());
            result += that_;
            return result;
        }
    };
}

namespace edt::geom {
    template<typename T, size_t Rows, size_t Columns>
    class _EMPTY_BASES Matrix :
        public details::data::DataMixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_data_access::VectorDataAccessMixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_rotation::VectorRotationMixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::cast::CastMixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::common::CommonMixin<T, Rows, Columns, ::edt::geom::Matrix>
    {
    public:
    };

    constexpr auto sz = sizeof(Matrix<float, 4, 4>);
    static_assert(sz == sizeof(float) * 4 * 4);
}
