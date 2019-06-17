#include "gtest/gtest.h"
#include "EverydayTools/Array/ArrayView.h"

TEST(ArrayViewTest, Constructors) {
    using namespace edt;

    {
        constexpr size_t cnt = 10;
        size_t instances[cnt];

        for (size_t i = 0; i < cnt; ++i) {
            instances[i] = i;
        }

        DenseArrayView<size_t> view(instances, cnt);

        for (size_t i = 0; i < cnt; ++i) {
            EXPECT_TRUE(view[i] == instances[i]);
        }
    }

    {
        constexpr size_t collectionSize = 10;

        struct Tst
        {
            size_t values[collectionSize];
        };

        constexpr size_t instancesCount = 20;
        Tst instances[instancesCount];

        {
            size_t i = 0, j = 0;
            for (auto& inst : instances) {
                ++i;
                for(auto& val: inst.values) {
                    val = i * collectionSize + j;
                }
            }
        }

        for (size_t i = 0; i < instancesCount; ++i) {
            for (size_t j = 0; j < collectionSize; ++j) {
                instances[i].values[j] = j * collectionSize + i;
            }
        }

        for (size_t i = 0; i < instancesCount; ++i) {
            SparseArrayView<size_t> view(&instances[0].values[i], instancesCount, sizeof(Tst));

            for (size_t j = 0; j < collectionSize; ++j) {
                EXPECT_TRUE(view[j] == instances[j].values[i]);
            }
        }
    }
}
