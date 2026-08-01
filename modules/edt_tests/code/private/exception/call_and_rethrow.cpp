#include "edt/exception/call_and_rethrow.hpp"

#include <exception>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "gtest/gtest.h"

namespace
{
// Collects the what() of an exception and everything nested inside it.
void Collect(const std::exception& e, std::vector<std::string>& out)
{
    out.emplace_back(e.what());
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const std::exception& nested)
    {
        Collect(nested, out);
    }
}
}  // namespace

TEST(CallAndRethrowTest, ReturnsTheCallableResult)
{
    EXPECT_EQ(edt::CallAndRethrow("ctx", [] { return 42; }), 42);
}

TEST(CallAndRethrowTest, HandlesVoidCallables)
{
    int side_effect = 0;
    edt::CallAndRethrow("ctx", [&] { side_effect = 1; });
    EXPECT_EQ(side_effect, 1);
}

TEST(CallAndRethrowTest, NestsTheOriginalException)
{
    std::vector<std::string> messages;
    try
    {
        edt::CallAndRethrow("outer context", [] { throw std::runtime_error("inner"); });
        FAIL() << "expected a throw";
    }
    catch (const std::exception& e)
    {
        Collect(e, messages);
    }

    ASSERT_EQ(messages.size(), 2U);
    EXPECT_EQ(messages[0], "outer context");
    EXPECT_EQ(messages[1], "inner");
}

// Without an explicit context the message comes from std::source_location.
TEST(CallAndRethrowTest, DefaultsContextToTheCallingFunction)
{
    try
    {
        edt::CallAndRethrow([] { throw std::runtime_error("inner"); });
        FAIL() << "expected a throw";
    }
    catch (const std::exception& e)
    {
        EXPECT_NE(std::string_view(e.what()).find("DefaultsContextToTheCallingFunction"), std::string_view::npos)
            << "actual: " << e.what();
    }
}
