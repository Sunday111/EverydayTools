#include "Matrix.hpp"

template <typename T, size_t R, size_t C>
struct SizeCheckMatrixTest {
  static constexpr bool Enabled = true;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx>;
  static void Execute(Mtx a) {
    EXPECT_TRUE(Equal(sizeof(a), sizeof(T) * R * C));
  }
};

TEST(MarixTest, SizeCheck) { RunTest<SizeCheckMatrixTest>(); }

template <typename T, size_t R, size_t C>
struct IndentityMarixTest {
  static constexpr bool Enabled = is_square<R, C>;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<>;
  static void Execute() {
    auto m = Mtx::Identity();
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        auto a = static_cast<T>(m.At(i, j));
        auto b = static_cast<T>(i == j ? 1 : 0);
        EXPECT_TRUE(Equal(a, b));
      }
    }
  }
};

TEST(MarixTest, Identity) { RunTest<IndentityMarixTest>(); }

template <typename T, size_t R, size_t C>
struct FillMarixTest {
  static constexpr bool Enabled = true;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx>;
  static void Execute(Mtx a) {
    T b = 10;
    a.Fill(b);
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        EXPECT_TRUE(Equal(a.At(i, j), b));
      }
    }
  }
};

TEST(MarixTest, Fill) { RunTest<FillMarixTest>(); }

template <typename T, size_t R, size_t C>
struct DotProductVectorTest {
  static constexpr bool Enabled = is_vector<R, C>;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx, Mtx>;
  static void Execute(Mtx a, Mtx b) {
    size_t N = std::max(R, C);
    T expected{};
    for (size_t i = 0; i < N; ++i) {
      expected += a.Elem(i) * b.Elem(i);
    }
    auto actual = a.Dot(b);
    EXPECT_TRUE(Equal<T>(expected, actual));
  }
};

TEST(VectorTest, Dot) { RunTest<FillMarixTest>(); }

template <typename T, size_t R, size_t C>
struct CrossProductVectorTest {
  static constexpr bool Enabled = is_nd_vector<R, C, 3>;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx, Mtx>;
  static void Execute(Mtx a, Mtx b) {
    auto c = a.Cross(b);
    EXPECT_TRUE(Equal<T>(a.y() * b.z() - a.z() * b.y(), c.x()));
    EXPECT_TRUE(Equal<T>(a.z() * b.x() - a.x() * b.z(), c.y()));
    EXPECT_TRUE(Equal<T>(a.x() * b.y() - a.y() * b.x(), c.z()));
  }
};

TEST(VectorTest, Cross) { RunTest<CrossProductVectorTest>(); }

template <typename T, size_t R, size_t C>
struct AddNumberMarixTest {
  static constexpr bool Enabled = true;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx>;
  static void Execute(Mtx a) {
    auto b = a;
    T val = static_cast<T>(std::rand());
    b.Add(val);
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        EXPECT_TRUE(Equal<T>(a.At(i, j) + val, b.At(i, j)));
      }
    }
  }
};

TEST(MarixTest, AddNumber) { RunTest<AddNumberMarixTest>(); }

template <typename T, size_t R, size_t C>
struct AddMatrixMarixTest {
  static constexpr bool Enabled = true;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx, Mtx>;
  static void Execute(Mtx a, Mtx b) {
    auto c = a + b;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        EXPECT_TRUE(Equal<T>(a.At(i, j) + b.At(i, j), c.At(i, j)));
      }
    }
  }
};

TEST(MarixTest, AddMatrix) { RunTest<AddMatrixMarixTest>(); }

template <typename T, size_t R, size_t C>
struct MultiplyNumberMarixTest {
  static constexpr bool Enabled = true;
  static constexpr bool RandomInput = true;
  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx, T>;
  static void Execute(Mtx a, T b) {
    auto c = a * b;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        EXPECT_TRUE(Equal<T>(a.At(i, j) * b, c.At(i, j)));
      }
    }
  }
};

TEST(MarixTest, MultiplyNumber) { RunTest<MultiplyNumberMarixTest>(); }

template <typename T, size_t R, size_t C>
struct MultiplyMatrixMarixTest {
  static constexpr bool Enabled = true;
  static constexpr bool RandomInput = true;

  using Mtx = Matrix<T, R, C>;
  using Input = std::tuple<Mtx>;

  template <size_t ColumnsB>
  static void ExecutePass(const Mtx& a) {
    Matrix<T, C, ColumnsB> b;
    RandomizeValue(b);
    auto c = a * b;
    size_t cr = c.RowsCount();
    size_t cc = c.ColumnsCount();

    // Check result matrix size
    EXPECT_TRUE(cr == R);
    EXPECT_TRUE(cc == ColumnsB);

    // Check result matrix values
    for (size_t ir = 0; ir < cr; ++ir) {
      for (size_t ic = 0; ic < cc; ++ic) {
        // DotProduct(a row 'ir', b column 'ic')
        T expect_value{};
        for (size_t i = 0; i < C; ++i) {
          auto val_a = a.At(ir, i);
          auto val_b = b.At(i, ic);
          expect_value += val_a * val_b;
        }

        auto actual_value = c.At(ir, ic);
        EXPECT_TRUE(Equal(actual_value, expect_value));
      }
    }
  }

  template <size_t... ColumnsB>
  static void ExecutePasses(const Mtx& a) {
    (ExecutePass<ColumnsB>(a), ...);
  }

  static void Execute(Mtx a) { ExecutePasses<1, 2, 3, 4, 5, 6>(a); }
};

TEST(MarixTest, MultiplyMatrix) { RunTest<MultiplyMatrixMarixTest>(); }
