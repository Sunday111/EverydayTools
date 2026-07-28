#include "edt/template/overload.hpp"

#include <string>
#include <variant>

#include "gtest/gtest.h"

// Class template argument deduction works for aggregates since C++20, so no
// explicit deduction guide is needed.
TEST(OverloadTest, DeducesWithoutADeductionGuide)
{
    const edt::Overload visitor{
        [](int v) { return "int:" + std::to_string(v); },
        [](double v) { return "double:" + std::to_string(static_cast<int>(v)); },
        [](const std::string& v) { return "string:" + v; },
    };

    std::variant<int, double, std::string> value = 42;
    EXPECT_EQ(std::visit(visitor, value), "int:42");

    value = 1.5;
    EXPECT_EQ(std::visit(visitor, value), "double:1");

    value = std::string("x");
    EXPECT_EQ(std::visit(visitor, value), "string:x");
}
