#include "EverydayTools/Math/Matrix.hpp"

#include <xmmintrin.h>

#include <EverydayTools/Math/Math.hpp>

namespace edt
{

static_assert(
    []
    {
        Matrix<int, 3, 3> m;
        for (size_t r = 0; r != 3; ++r)
        {
            for (size_t c = 0; c != 3; ++c)
            {
                if (m(r, c) != 0)
                {
                    return false;
                }
            }
        }

        m(0, 1) = 42;
        m(1, 0) = 24;

        if (m(0, 1) != 42) return false;
        if (m(1, 0) != 24) return false;

        return true;
    }());

static_assert(
    []
    {
        auto m = Matrix<int, 5, 3>::Identity();

        int s = 0;
        for (size_t c = 0; c != m.NumColumns(); ++c)
        {
            for (size_t r = 0; r != m.NumRows(); ++r)
            {
                s += m(r, c);
                if (c == r && m(r, c) != 1) return false;
            }
        }

        return s == 3;
    }(),
    "Identity test");

static_assert(
    []
    {
        Vec3i a{{1, 2, 3}};
        Vec3i b{{4, 5, 6}};
        return a.Dot(b) == 1 * 4 + 2 * 5 + 3 * 6;
    }(),
    "Dot product test");

static_assert(
    []
    {
        Vec3i a{{1, 2, 3}};
        Vec3i b{{4, 5, 6}};
        return a.Cross(b) == Vec3i{{-3, 6, -3}};
    }(),
    "Cross product test");

static_assert(
    []
    {
        // clang-format off
        Matrix<int, 4, 3> a{{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11
        }};
        Matrix<int, 3, 5> b{{
            14, 13, 12, 11, 10,
            9, 8, 7, 6, 5,
            4, 3, 2, 1, 0
        }};
        Matrix<int, 4, 5> expected{{
            17, 14, 11, 8, 5,
            98, 86, 74, 62, 50,
            179, 158, 137, 116, 95,
            260, 230, 200, 170, 140
        }};
        // clang-format on

        Matrix<int, 4, 5> actual = a.MatMul(b);
        return actual == expected;
    }(),
    "Matrix multiplication test");

static_assert(
    []
    {
        // clang-format off
        Matrix<int, 3, 4> m{{
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11
        }};
        Matrix<int, 1, 4> expected{{
            4, 5, 6, 7,
        }};
        // clang-format on

        Matrix<int, 1, 4> actual = m.GetRow(1);
        return actual == expected;
    }(),
    "GetRow Test");

static_assert(
    []
    {
        // clang-format off
        Matrix<int, 3, 4> m{{
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11
        }};
        Matrix<int, 3, 1> expected{{
            1, 5, 9
        }};
        // clang-format on

        Matrix<int, 3, 1> actual = m.GetColumn(1);
        return actual == expected;
    }(),
    "GetColumn Test");

static_assert(
    []
    {
        // clang-format off
        Matrix<int, 3, 4> m{{
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11
        }};
        Matrix<int, 4, 3> expected{{
            0, 4, 8,
            1, 5, 9,
            2, 6, 10,
            3, 7, 11
        }};
        // clang-format on

        Matrix<int, 4, 3> actual = m.Transposed();
        return actual == expected;
    }(),
    "Transposed Test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = a + 10;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != static_cast<int>(row * b.NumColumns() + column) + 10)
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Matrix + scalar test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = 10 + a;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != static_cast<int>(row * b.NumColumns() + column) + 10)
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Scalar + matrix test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = a - 10;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != static_cast<int>(row * b.NumColumns() + column) - 10)
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Matrix minus scalar test");

static_assert(
    []
    {
        Matrix<size_t, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = 10uz - a;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != 10uz - (row * b.NumColumns() + column))
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Scalar minus matrix test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = a * 10;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != static_cast<int>(row * b.NumColumns() + column) * 10)
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Matrix times scalar test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = 10 * a;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != 10 * static_cast<int>(row * b.NumColumns() + column))
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Scalar times matrix test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};

        auto b = a / 10;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != static_cast<int>(row * b.NumColumns() + column) / 10)
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Matrix divided by scalar test");

static_assert(
    []
    {
        Matrix<int, 5, 3> a{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};

        auto b = 10 / a;

        for (const size_t row : b.RowIndices())
        {
            for (const size_t column : b.ColumnIndices())
            {
                if (b(row, column) != 10 / static_cast<int>(row * b.NumColumns() + column + 1))
                {
                    return false;
                }
            }
        }

        return true;
    }(),
    "Scalar divided by matrix test");

static_assert(
    []
    {
        auto m = edt::Matrix<int, 4, 4>::Identity();
        m(0, 3) = 5;
        m(1, 3) = 8;
        m(2, 3) = 19;
        edt::Vec4<int> v{0, 0, 0, 1};
        auto translated = m.MatMul(v);
        return translated == Vec4<int>{5, 8, 19, 1};
    }(),
    "Translation matrix test");

// Rotation around axis X, CCW
static_assert(
    []
    {
        const auto m = edt::Math::RotationMatrix3dX(edt::Math::DegToRad(90.f));
        const auto v = edt::Math::TransformVector(m, {0, 1, 0});
        return std::abs(v.x()) < 0.1f && std::abs(v.y()) < 0.1f && std::abs(v.z() - 1) < 0.1f;
    }(),
    "Rotation around X axis test");

// Rotation around axis Y, CCW
static_assert(
    []
    {
        const auto m = edt::Math::RotationMatrix3dY(edt::Math::DegToRad(90.f));
        const auto v = edt::Math::TransformVector(m, {1, 0, 0});
        return std::abs(v.x()) < 0.1f && std::abs(v.y()) < 0.1f && std::abs(v.z() + 1) < 0.1f;
    }(),
    "Rotation around Y axis test");

// Rotation around axis Z, CCW
static_assert(
    []
    {
        const auto m = edt::Math::RotationMatrix3dZ(edt::Math::DegToRad(90.f));
        const auto v = edt::Math::TransformVector(m, {1, 0, 0});
        return std::abs(v.x()) < 0.1f && std::abs(v.y() - 1) < 0.1f && std::abs(v.z()) < 0.1f;
    }(),
    "Rotation around Y axis test");

// Rotation around axis X, CCW and then translation
// Translation part should be ignored for vector operation
static_assert(
    []
    {
        const auto m = edt::Math::TranslationMatrix(Vec3f{41, 21, 13})
                           .MatMul(edt::Math::RotationMatrix3dX(edt::Math::DegToRad(90.f)));
        const auto v = edt::Math::TransformVector(m, {0, 1, 0});
        return std::abs(v.x()) < 0.1f && std::abs(v.y()) < 0.1f && std::abs(v.z() - 1) < 0.1f;
    }(),
    "Rotation around X axis test");

// Rotation around axis X, CCW and then translation
// Translation part should not be ignored for vector operation
static_assert(
    []
    {
        const auto m = edt::Math::TranslationMatrix(Vec3f{41, 21, 13})
                           .MatMul(edt::Math::RotationMatrix3dX(edt::Math::DegToRad(90.f)));
        const auto v = edt::Math::TransformPos(m, {0, 1, 0});
        return std::abs(v.x() - 41.f) < 0.1f && std::abs(v.y() - 21.f) < 0.1f && std::abs(v.z() - 14.f) < 0.1f;
    }(),
    "Rotation around X axis test");

// void M4x4_SSE(const float* a, const float* b, float* c)
// {
//     __m128 row1 = _mm_load_ps(&b[0]);
//     __m128 row2 = _mm_load_ps(&b[4]);
//     __m128 row3 = _mm_load_ps(&b[8]);
//     __m128 row4 = _mm_load_ps(&b[12]);
//     for (int i = 0; i < 4; i++)
//     {
//         __m128 brod1 = _mm_set1_ps(a[4 * i + 0]);
//         __m128 brod2 = _mm_set1_ps(a[4 * i + 1]);
//         __m128 brod3 = _mm_set1_ps(a[4 * i + 2]);
//         __m128 brod4 = _mm_set1_ps(a[4 * i + 3]);
//         __m128 row = _mm_add_ps(
//             _mm_add_ps(_mm_mul_ps(brod1, row1), _mm_mul_ps(brod2, row2)),
//             _mm_add_ps(_mm_mul_ps(brod3, row3), _mm_mul_ps(brod4, row4)));
//         _mm_store_ps(&c[4 * i], row);
//     }
// }

// [[nodiscard]] edt::Mat4f M4x4_SSE(const edt::Mat4f& a, const edt::Mat4f& b)
// {
//     edt::Mat4f mc;
//     __m128 r0 = _mm_load_ps(&b.At<0, 0>());
//     __m128 r1 = _mm_load_ps(&b.At<1, 0>());
//     __m128 r3 = _mm_load_ps(&b.At<2, 0>());
//     __m128 r4 = _mm_load_ps(&b.At<3, 0>());

//     _mm_store_ps(
//         &mc.At<0, 0>(),
//         _mm_add_ps(
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<0, 0>()), r0), _mm_mul_ps(_mm_set1_ps(a.At<0, 1>()), r1)),
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<0, 2>()), r3), _mm_mul_ps(_mm_set1_ps(a.At<0, 3>()), r4))));

//     _mm_store_ps(
//         &mc.At<1, 0>(),
//         _mm_add_ps(
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<1, 0>()), r0), _mm_mul_ps(_mm_set1_ps(a.At<1, 1>()), r1)),
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<1, 2>()), r3), _mm_mul_ps(_mm_set1_ps(a.At<1, 3>()), r4))));

//     _mm_store_ps(
//         &mc.At<2, 0>(),
//         _mm_add_ps(
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<2, 0>()), r0), _mm_mul_ps(_mm_set1_ps(a.At<2, 1>()), r1)),
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<2, 2>()), r3), _mm_mul_ps(_mm_set1_ps(a.At<2, 3>()), r4))));

//     _mm_store_ps(
//         &mc.At<3, 0>(),
//         _mm_add_ps(
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<3, 0>()), r0), _mm_mul_ps(_mm_set1_ps(a.At<3, 1>()), r1)),
//             _mm_add_ps(_mm_mul_ps(_mm_set1_ps(a.At<3, 2>()), r3), _mm_mul_ps(_mm_set1_ps(a.At<3, 3>()), r4))));

//     return mc;
// }

// TEST(MatrixTest, MultiplicationSSE)
// {
//     static constexpr unsigned kSeed = 12345;
//     std::mt19937 rnd(kSeed);  // NOLINT
//     std::uniform_real_distribution<float> distr(-1.f, 1.f);

//     edt::Mat4f a;
//     edt::Mat4f b;
//     edt::Mat4f expected;
//     edt::Mat4f actual;

//     auto rnd_mtx = [&](edt::Mat4f& m)
//     {
//         for (size_t i = 0; i != m.data_.size(); ++i)
//         {
//             m.data_[i] = distr(rnd);
//         }
//     };

//     for (size_t i = 0; i != 1000; ++i)
//     {
//         rnd_mtx(a);
//         rnd_mtx(b);

//         expected = a.MatMul(b);
//         actual = M4x4_SSE(a, b);

//         for (size_t j = 0; j != a.data_.size(); ++j)
//         {
//             ASSERT_NEAR(expected.data_[j], actual.data_[j], 0.0001f);
//         }
//     }
// }
}  // namespace edt
