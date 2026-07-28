#pragma once

#include <concepts>
#include <numbers>

#include "../float_aliases.hpp"
#include "math.hpp"

namespace edt
{
class SurfacePointsUtilities
{
public:
    // Generates specified number of points evenly distributed on the surface of sphere
    // It is impossible to do that perfectly for n > 50 so this is just an approximation
    template <std::invocable<Vec3f> F>
    inline static void UniformSphereSurface(size_t num_points, f32 radius, F consumer)
    {
        const f64 k = std::numbers::pi * 4.0 / (std::sqrt(5.0) + 1);
        const f64 dn = static_cast<f64>(num_points);
        for (size_t i = 0; i != num_points; ++i)
        {
            f64 di = static_cast<f64>(i);
            f64 theta = di * k;
            f64 cos_phi = 1 - 2 * di / dn;
            f64 sin_phi = std::sqrt(1 - Math::Sqr(cos_phi));
            f64 x = std::cos(theta) * sin_phi;
            f64 y = std::sin(theta) * sin_phi;
            consumer(Vec3<f64>{x, y, cos_phi}.Cast<f32>() * radius);
        }
    }

    // Julia's parametric heart surface
    // To avoid confusion with periods of i and j they are accepted in range [0; 1)
    inline static Vec3f HeartFunction3d(edt::Vec2f ij)
    {
        constexpr Vec2f uv_period{
            2 * std::numbers::pi_v<f32>,
            std::numbers::pi_v<f32>,
        };

        const auto [u, v] = (ij * uv_period).Tuple();
        return {
            std::sin(v) * (15.f * std::sin(u) - 4.f * std::sin(3.f * u)),
            8 * std::cos(v),
            std::sin(v) * (15 * std::cos(u) - 5 * std::cos(2 * u) - 2 * std::cos(3 * u) - std::cos(4 * u)),
        };
    }

    // Generates specified number of points that cover heart-like surface
    template <std::invocable<edt::Vec3f> F>
    static void HeartSurface(size_t num_points, f32 scale, F consumer)
    {
        if (num_points == 0) return;

        size_t generated = 0;
        size_t s = static_cast<size_t>(std::round(std::sqrt(static_cast<f32>(num_points))));
        size_t max_j = s;
        size_t max_i = s * s < num_points ? s + 1 : s;
        for (const size_t i : std::views::iota(size_t{0}, max_i))
        {
            const f32 fi = static_cast<f32>(i) / static_cast<f32>(max_i);
            for (const size_t j : std::views::iota(size_t{0}, max_j))
            {
                const f32 fj = static_cast<f32>(j) / static_cast<f32>(max_j);
                Vec3f point = HeartFunction3d({fi, fj}) * scale;
                consumer(point);
                if (++generated == num_points) return;
            }
        }
    }
};
}  // namespace edt
