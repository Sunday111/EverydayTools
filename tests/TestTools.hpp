#pragma once

#include <random>
#include <stdexcept>
#include <sstream>
#include <vector>

template<typename T>
using Distribution = std::conditional_t<
	std::is_integral<T>::value,
	std::uniform_int_distribution<T>,
	std::uniform_real_distribution<T>>;

template<typename T, typename Engine, typename F>
void GenerateRandomNumbers(T min, T max, size_t count, Engine& engine, F&& f)
{

	Distribution<T> valuesDistribution(min, max);

	for (size_t i = 0; i < count; ++i)
	{
		f(valuesDistribution(engine));
	}
}

template<typename Engine>
size_t GenerateRandomSize(size_t min, size_t max, Engine& engine)
{
	size_t result = min;
	Distribution<size_t> sizeDistribution(min, max);
	GenerateRandomNumbers<size_t>(min, max, 1, engine, [&](size_t value)
	{
		result = value;
	});

	return result;
}

template<typename T>
std::vector<T> GenerateRandomArray(size_t minSize, size_t maxSize, T minValue, T maxValue)
{
	std::random_device rd;
	std::mt19937 mt(rd());

	std::vector<T> result;
	auto size = GenerateRandomSize(minSize, maxSize, mt);
	result.reserve(size);

	GenerateRandomNumbers<T>(minValue, maxValue, size, mt, [&](T value)
	{
		result.push_back(value);
	});

	return result;
}