#pragma once

#include <limits>

namespace edt
{

template <typename Tag, typename Repr_, Repr_ invalid_id = std::numeric_limits<Repr_>::max()>
class TaggedIdentifier
{
public:
    using Repr = Repr_;

private:
    explicit constexpr TaggedIdentifier(const Repr& value) noexcept : value_(value) {}

public:
    constexpr TaggedIdentifier() noexcept = default;
    constexpr TaggedIdentifier(const TaggedIdentifier&) noexcept = default;
    constexpr TaggedIdentifier(TaggedIdentifier&&) noexcept = default;
    constexpr TaggedIdentifier& operator=(const TaggedIdentifier&) noexcept = default;
    constexpr TaggedIdentifier& operator=(TaggedIdentifier&&) noexcept = default;

    static constexpr TaggedIdentifier FromValue(const Repr& repr)
    {
        return TaggedIdentifier(repr);
    }

    constexpr const Repr& GetValue() const noexcept
    {
        return value_;
    }

    constexpr bool IsValid() const noexcept
    {
        return value_ != invalid_id;
    }

    constexpr bool operator==(const TaggedIdentifier& id) const noexcept
    {
        return value_ == id.value_;
    }

    constexpr bool operator!=(const TaggedIdentifier& id) const noexcept
    {
        return value_ != id.value_;
    }

    constexpr bool operator<(const TaggedIdentifier& id) const noexcept
    {
        return value_ < id.value_;
    }

    constexpr bool operator>(const TaggedIdentifier& id) const noexcept
    {
        return value_ > id.value_;
    }

private:
    Repr value_ = invalid_id;
};

}  // namespace edt
