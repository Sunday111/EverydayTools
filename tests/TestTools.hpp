#pragma once

#include <random>
#include <stdexcept>
#include <vector>

#include "EverydayTools/Math/Matrix.hpp"

template <typename T>
using Distribution =
    std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;

template <typename T, typename Engine, typename F>
void GenerateRandomNumbers(T min, T max, size_t count, Engine& engine, F&& f)
{
    Distribution<T> valuesDistribution(min, max);

    for (size_t i = 0; i < count; ++i)
    {
        f(valuesDistribution(engine));
    }
}

template <typename Engine>
size_t GenerateRandomSize(size_t min, size_t max, Engine& engine)
{
    size_t result = min;
    Distribution<size_t> sizeDistribution(min, max);
    GenerateRandomNumbers<size_t>(min, max, 1, engine, [&](size_t value) { result = value; });

    return result;
}

template <typename T>
std::vector<T> GenerateRandomArray(size_t minSize, size_t maxSize, T minValue, T maxValue)
{
    std::random_device rd;
    std::mt19937 mt(rd());

    std::vector<T> result;
    auto size = GenerateRandomSize(minSize, maxSize, mt);
    result.reserve(size);

    GenerateRandomNumbers<T>(minValue, maxValue, size, mt, [&](T value) { result.push_back(value); });

    return result;
}

template <typename T>
[[nodiscard]] constexpr T CAbs(T v)
{
    return v < 0 ? -v : v;
}

template <std::floating_point T1, std::convertible_to<T1> T2>
constexpr void ExpectEq(T1 a, T2 b, T1 precision = T1{0.0001})
{
    auto delta = a - b;
    if (delta < 0) delta = -delta;

    if (delta < precision) return;
    throw std::runtime_error("Check failed");
}

template <std::floating_point T, size_t rows, size_t columns>
constexpr void ExpectEq(edt::Matrix<T, rows, columns> a, edt::Matrix<T, rows, columns> b, T precision = T{0.0001})
{
    for (size_t r = 0; r != rows; ++r)
    {
        for (size_t c = 0; c != columns; ++c)
        {
            ExpectEq(a(r, c), b(r, c), precision);
        }
    }
}
