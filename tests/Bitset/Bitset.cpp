#include "EverydayTools/Bitset/Bitset.hpp"

#include <random>
#include <vector>

#include "gtest/gtest.h"

using namespace edt;

TEST(BitsetTest, Construction)  // NOLINT
{
    [[maybe_unused]] Bitset a;
}

TEST(BitsetTest, ResizeSetGet)  // NOLINT
{
    Bitset bitset;
    std::vector<bool> vector;

    constexpr unsigned kSeed = 0;
    constexpr size_t kResizesCount = 1'000;
    constexpr size_t kMinSize = 1;
    constexpr size_t kMaxSize = 1'000;
    constexpr size_t kMinFlipsCount = 50;
    constexpr size_t kMaxFlipsCount = 100;
    std::mt19937 generator(kSeed);  // NOLINT
    std::uniform_int_distribution<size_t> size_distributor(kMinSize, kMaxSize);
    std::uniform_int_distribution<size_t> flips_count_distributor(kMinFlipsCount, kMaxFlipsCount);

    for (size_t resize_index = 0; resize_index != kResizesCount; ++resize_index)
    {
        const size_t new_size = size_distributor(generator);

        vector.resize(new_size);
        ASSERT_EQ(vector.size(), new_size);

        bitset.Resize(new_size, false);
        ASSERT_EQ(bitset.Size(), vector.size());

        for (size_t index = 0; index != vector.size(); ++index)
        {
            ASSERT_EQ(vector[index], bitset.Get(index))
                << "resize_index = " << resize_index << ", bit_index = " << index;
        }

        std::uniform_int_distribution<size_t> index_distributor(0, new_size - 1);
        const size_t flips_count = flips_count_distributor(generator) % new_size;
        for (size_t flip_index = 0; flip_index != flips_count; ++flip_index)
        {
            const bool prev_value = vector[flip_index];
            const bool new_value = !prev_value;

            vector[flip_index] = new_value;
            ASSERT_EQ(vector[flip_index], new_value);

            ASSERT_EQ(bitset.Get(flip_index), prev_value);
            bitset.Set(flip_index, new_value);
            ASSERT_EQ(bitset.Get(flip_index), new_value);

            for (size_t index = 0; index != vector.size(); ++index)
            {
                ASSERT_EQ(vector[index], bitset.Get(index))
                    << "resize_index = " << resize_index << ", bit_index = " << index
                    << ", flip index = " << flip_index;
            }
        }
    }
}
