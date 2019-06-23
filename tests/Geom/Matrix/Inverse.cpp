#include "gtest/gtest.h"
#include "EverydayTools/Algorithm/Compare.h"
#include "EverydayTools/Geom/Matrix.h"
#include "EverydayTools/UnusedVar.h"
#include "Matrix.h"
#include <optional>

template<typename T, size_t N>
constexpr bool GaussJordanElimination(edt::geom::Matrix<T, N, 2 * N>& mtx, size_t(&rowIdx)[N]) {
    for (size_t i = 0; i < N; ++i) { rowIdx[i] = i; }

    auto reindex = [&rowIdx, &mtx]() {
        edt::geom::Matrix<T, N, 2 * N> r{};
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < 2 * N; ++j) {
                r.At(i, j) = mtx.At(rowIdx[i], j);
            }
        }

        return r;
    };

    for (size_t j = 0; j < N; ++j) {
        // Find the row i with i>=j such that Mij hast the largest absolute value.
        std::optional<size_t> maybe_i;

        {
            T maxAbs = 0;
            for (size_t i = j; i < N; ++i) {
                const T& absCell = std::abs(mtx.At(rowIdx[i], j));
                if (absCell > maxAbs) {
                    maxAbs = absCell;
                    maybe_i = i;
                }
            }

            // If no such row exists for which Mij != 0, then M is not invertible
            if (!maybe_i.has_value() || edt::Equals<T>(maxAbs, 0)) {
                return false;
            }
        }

        const T& foundValue = mtx.At(rowIdx[*maybe_i], j);

        // Exchange rows i and j
        if (*maybe_i != j) {
            std::swap(rowIdx[*maybe_i], rowIdx[j]);
        }

        // Multiply row j by 1 / maxAbs. This sets (j, j) entry of M to 1
        mtx.GetRow(rowIdx[j]) *= (1 / foundValue);

        for (size_t r = 0; r < N; ++r) {
            if (r != j) {
                const T& k = -mtx.At(rowIdx[r], j);
                mtx.GetRow(rowIdx[r]) += mtx.GetRow(rowIdx[j]) * k;
            }
        }
    }

    return true;
}

TEST(MarixTest, Inverse) {
    using namespace edt::geom;
    using T = float;
    constexpr size_t N = 3;
    using M = Matrix<T, N, N>;

    M source;
    for (size_t p = 0; p < 100; ++p) {
        Randomizer<M>::Randomize(source);

        // Construct the augmented n x 2n matrix M
        // by concatenating the identity matrix to the right of M
        Matrix<T, N, 2 * N> augmented;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                augmented.At(i, j) = source.At(i, j);
                augmented.At(i, j + N) = static_cast<T>(i == j ? 1 : 0);
            }
        }

        size_t rowIdx[N];
        if (GaussJordanElimination(augmented, rowIdx)) {
            // Unpack
            Matrix<T, N, N> inverted, identity;
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    identity.At(i, j) = augmented.At(rowIdx[i], j);
                    inverted.At(i, j) = augmented.At(rowIdx[i], j + N);
                }
            }

            auto isIdentity = [](const Matrix<T, N, N>& matrix) {
                for (size_t i = 0; i < matrix.RowsCount(); ++i) {
                    for (size_t j = 0; j < matrix.ColumnsCount(); ++j) {
                        if (!edt::Equals<T>(matrix.At(i, j), (i == j ? 1.0f : 0.0f), 0.001f)) {
                            return false;
                        }
                    }
                }

                return true;
            };

            // Check identity
            if (!isIdentity(identity)) {
                EXPECT_TRUE(!"Should be identity");
            }

            // Check equality
            auto multiplied = source * inverted;
            if (!isIdentity(multiplied)) {
                EXPECT_TRUE(!"Should be identity");
            }
        }
    }
}