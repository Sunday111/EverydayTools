#pragma once

#include <compare>
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

    static constexpr TaggedIdentifier FromValue(const Repr& repr) noexcept { return TaggedIdentifier(repr); }

    [[nodiscard]] constexpr const Repr& GetValue() const noexcept { return value_; }

    [[nodiscard]] constexpr bool IsValid() const noexcept { return value_ != invalid_id; }

    [[nodiscard]] friend constexpr bool operator==(const TaggedIdentifier&, const TaggedIdentifier&) = default;
    [[nodiscard]] friend constexpr auto operator<=>(const TaggedIdentifier&, const TaggedIdentifier&) = default;

private:
    Repr value_ = invalid_id;
};

}  // namespace edt
