#include "EverydayTools/Array/ArrayView.hpp"
#include "gtest/gtest.h"

TEST(ArrayViewTest, CastAndConversion) {
  using namespace edt;

  struct Base {
    size_t base;
  };

  struct Derived : public Base {
    size_t derived;
  };

  constexpr size_t cnt = 3;
  Derived instances[cnt];

  for (size_t i = 0; i < cnt; ++i) {
    instances[i].base = i;
    instances[i].derived = cnt + i;
  }

  DenseArrayView<Derived> derivedView(instances, cnt);

  // May be casted to constant version
  DenseArrayView<const Derived> constDerived = derivedView;

  // May be casted to base type view
  SparseArrayView<Base> baseView = derivedView;

  for (size_t i = 0; i < cnt; ++i) {
    EXPECT_TRUE(derivedView[i].base == baseView[i].base);
    EXPECT_TRUE(constDerived[i].base == baseView[i].base);
  }
}
