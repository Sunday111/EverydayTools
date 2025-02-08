#pragma once

#include <concepts>
#include <numbers>

#include "Math.hpp"

namespace edt
{
class SurfacePointsUtilities
{
public:
    // Generates specified number of points evenly distributed on the surface of sphere
    // It is impossible to do that perfectly for n > 50 so this is just an approximation
    template <std::invocable<Vec3f> F>
    inline static void UniformSphereSurface(size_t num_points, float radius, F consumer)
    {
        const double k = std::numbers::pi * 4.0 / (std::sqrt(5.0) + 1);
        const double dn = static_cast<double>(num_points);
        for (size_t i = 0; i != num_points; ++i)
        {
            double di = static_cast<double>(i);
            double theta = di * k;
            double cos_phi = 1 - 2 * di / dn;
            double sin_phi = std::sqrt(1 - Math::Sqr(cos_phi));
            double x = std::cos(theta) * sin_phi;
            double y = std::sin(theta) * sin_phi;
            consumer(Vec3<double>{x, y, cos_phi}.Cast<float>() * radius);
        }
    }

    // Julia's parametric heart surface
    // To avoid confusion with periods of i and j they are accepted in range [0; 1)
    inline static Vec3f HeartFunction3d(edt::Vec2f ij)
    {
        constexpr Vec2f uv_period{
            2 * std::numbers::pi_v<float>,
            std::numbers::pi_v<float>,
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
    static void HeartSurface(size_t num_points, float scale, F consumer)
    {
        if (num_points == 0) return;

        size_t generated = 0;
        size_t s = static_cast<size_t>(std::round(std::sqrt(static_cast<float>(num_points))));
        size_t max_j = s;
        size_t max_i = s * s < num_points ? s + 1 : s;
        for (const size_t i : std::views::iota(size_t{0}, max_i))
        {
            const float fi = static_cast<float>(i) / static_cast<float>(max_i);
            for (const size_t j : std::views::iota(size_t{0}, max_j))
            {
                const float fj = static_cast<float>(j) / static_cast<float>(max_j);
                Vec3f point = HeartFunction3d({fi, fj}) * scale;
                consumer(point);
                if (++generated == num_points) return;
            }
        }
    }
};
}  // namespace edt
