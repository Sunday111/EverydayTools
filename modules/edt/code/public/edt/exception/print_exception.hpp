#pragma once

#include <cstddef>
#include <exception>
#include <format>
#include <iterator>
#include <ostream>

namespace edt
{
inline void PrintException(std::ostream& output, const std::exception& e, std::size_t nesting_depth = 0)
{
    auto out = std::ostreambuf_iterator<char>(output);
    std::format_to(out, "{:{}}exception: {}\n", "", nesting_depth, e.what());

    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const std::exception& nested)
    {
        PrintException(output, nested, nesting_depth + 1);
    }
    catch (...)
    {
        std::format_to(out, "{:{}}exception: unknown\n", "", nesting_depth + 1);
    }
}
}  // namespace edt
