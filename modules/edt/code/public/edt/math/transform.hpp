#pragma once

#include "math.hpp"
#include "rotator.hpp"

namespace edt
{
struct Transform
{
    [[nodiscard]] constexpr Mat4f Matrix() const noexcept
    {
        return Math::TranslationMatrix(translation).MatMul(rotation.ToMatrix().MatMul(Math::ScaleMatrix(scale)));
    }

    Vec3f translation{};
    Rotator rotation{};
    Vec3f scale{1, 1, 1};
};
}  // namespace edt
