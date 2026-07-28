#include "edt/exception/print_exception.hpp"

#include <sstream>
#include <stdexcept>

#include "gtest/gtest.h"

TEST(PrintExceptionTest, PrintsASingleException)
{
    std::ostringstream out;
    edt::PrintException(out, std::runtime_error("boom"));
    EXPECT_EQ(out.str(), "exception: boom\n");
}

TEST(PrintExceptionTest, IndentsNestedExceptions)
{
    std::ostringstream out;
    try
    {
        try
        {
            throw std::runtime_error("inner");
        }
        catch (...)
        {
            std::throw_with_nested(std::runtime_error("outer"));
        }
    }
    catch (const std::exception& e)
    {
        edt::PrintException(out, e);
    }

    EXPECT_EQ(out.str(), "exception: outer\n exception: inner\n");
}
