#include "edt/template/string_literal.hpp"

#include "gtest/gtest.h"

namespace
{
template <edt::StringLiteral name>
constexpr std::string_view NameOf()
{
    return name.GetView();
}
}  // namespace

TEST(StringLiteralTest, UsableAsNonTypeTemplateParameter)
{
    static_assert(NameOf<"hello">() == "hello");
    EXPECT_EQ(NameOf<"hello">(), "hello");
}

TEST(StringLiteralTest, ViewExcludesTheNullTerminator)
{
    constexpr edt::StringLiteral s("abc");
    static_assert(s.GetView().size() == 3);
    static_assert(s.Size() == 3);
    static_assert(s.GetView() == "abc");
}

TEST(StringLiteralTest, IsComparable)
{
    static_assert(edt::StringLiteral("abc") == edt::StringLiteral("abc"));
    static_assert(edt::StringLiteral("abc") != edt::StringLiteral("abd"));
    static_assert(edt::StringLiteral("abc") < edt::StringLiteral("abd"));
}
