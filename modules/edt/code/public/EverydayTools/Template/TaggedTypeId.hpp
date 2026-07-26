#pragma once

#include <cstddef>

#include "TaggedIdentifier.hpp"

namespace edt
{

template <typename Tag, typename Repr>
inline TaggedIdentifier<Tag, Repr> MakeNextTypeId()
{
    static Repr next_type_id = 0;
    return TaggedIdentifier<Tag, Repr>::FromValue(next_type_id++);
}

template <typename Tag, typename Repr, typename T>
inline TaggedIdentifier<Tag, Repr> GetTypeId()
{
    static const auto type_id = MakeNextTypeId<Tag, Repr>();
    return type_id;
}

}  // namespace edt
