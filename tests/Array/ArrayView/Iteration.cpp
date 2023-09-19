#include "EverydayTools/Array/ArrayView.hpp"
#include "TestTools.hpp"
#include "gtest/gtest.h"

constexpr size_t minSize = 0;
constexpr size_t maxSize = 10000;

template <typename T>
constexpr T minValue = static_cast<T>(0);

template <typename T>
constexpr T maxValue = static_cast<T>(10);

template <typename T, template <typename T_> typename ViewType>
struct ViewHelpers;

template <typename T>
struct ViewHelpers<T, edt::DenseArrayView>
{
    using Final = edt::DenseArrayView<T>;

    static decltype(auto) MakeCollection()
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        size_t size = GenerateRandomSize(minSize, maxSize, mt);
        std::vector<T> arr;
        arr.resize(size);
        return arr;
    }

    static void FillCollection(std::vector<T>& collection)
    {
        size_t index = 0;

        std::random_device rd;
        std::mt19937 mt(rd());
        GenerateRandomNumbers(
            minValue<T>,
            maxValue<T>,
            collection.size(),
            mt,
            [&](T value)
            {
                collection[index++] = value;
            });
    }

    static Final MakeView(std::vector<T>& collection)
    {
        auto ptr = collection.empty() ? nullptr : &collection[0];
        return Final(ptr, collection.size());
    }

    static bool Compare(T orig, T test)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::abs(orig - test) < static_cast<T>(0.000001);
        }
        else
        {
            return orig == test;
        }
    }

    static const char* GetViewName()
    {
        return "DenseArrayView";
    }
};

template <typename T>
struct ViewHelpers<T, edt::SparseArrayView>
{
    using Final = edt::SparseArrayView<T>;

    struct Dummy
    {
        int a;
        T value;
        double b;
    };

    static decltype(auto) MakeCollection()
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        size_t size = GenerateRandomSize(minSize, maxSize, mt);
        std::vector<Dummy> arr;
        arr.resize(size);
        return arr;
    }

    static void FillCollection(std::vector<Dummy>& collection)
    {
        size_t index = 0;

        std::random_device rd;
        std::mt19937 mt(rd());
        GenerateRandomNumbers(
            minValue<T>,
            maxValue<T>,
            collection.size(),
            mt,
            [&](T value)
            {
                collection[index++].value = value;
            });
    }

    static Final MakeView(std::vector<Dummy>& collection)
    {
        auto ptr = collection.empty() ? nullptr : &(collection[0].value);
        return Final(ptr, collection.size(), sizeof(Dummy));
    }

    static bool Compare(T orig, Dummy& test)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::abs(orig - test.value) < static_cast<T>(0.000001);
        }
        else
        {
            return orig == test.value;
        }
    }

    static const char* GetViewName()
    {
        return "SparseArrayView";
    }
};

template <typename T, template <typename T_> typename ViewType>
void IteratorsTest()
{
    using namespace edt;
    using Helper = ViewHelpers<T, ViewType>;

    auto arr = Helper::MakeCollection();
    Helper::FillCollection(arr);
    auto view = Helper::MakeView(arr);

    auto checkEquality = [&]()
    {
        auto it = view.begin();
        auto rit = view.rbegin();
        auto it_ = arr.begin();
        auto rit_ = arr.rbegin();

        while (!(it == view.end()) && rit != view.rend() && it_ != arr.end() && rit_ != arr.rend())
        {
            EXPECT_TRUE(Helper::Compare(*it, *it_));
            EXPECT_TRUE(Helper::Compare(*rit, *rit_));

            ++it;
            ++rit;
            ++it_;
            ++rit_;
        }
    };

    checkEquality();
    Helper::FillCollection(arr);
    checkEquality();
}

template <typename T>
void DenseIteratorsTest()
{
    IteratorsTest<T, edt::DenseArrayView>();
}

template <typename T>
void SparseIteratorsTest()
{
    IteratorsTest<T, edt::SparseArrayView>();
}

TEST(ArrayViewTest, Iteration)
{
    DenseIteratorsTest<int>();
    DenseIteratorsTest<long>();
    DenseIteratorsTest<float>();
    DenseIteratorsTest<double>();
    SparseIteratorsTest<int>();
    SparseIteratorsTest<long>();
    SparseIteratorsTest<float>();
    SparseIteratorsTest<double>();
}
