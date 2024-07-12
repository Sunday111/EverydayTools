#pragma once

#include "GUID.hpp"
#include "fmt/format.h"

template <>
struct fmt::formatter<edt::GUID> : fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const edt::GUID& guid, FormatContext& ctx)
    {
        const auto data = guid.ToCharArray();
        return fmt::format_to(ctx.out(), "{}", std::string_view(data.data(), data.size()));
    }
};
