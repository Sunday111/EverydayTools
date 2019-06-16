#pragma once

#include "ArrayView.h"

namespace edt
{
    namespace strview_detail
    {
        template<typename T>
        [[nodiscard]]
        constexpr size_t Strlen(const T* p) {
            if (p) {
                size_t i = 0;
                while (p[i] != 0) {
                    ++i;
                }

                return i;
            }

            return 0;
        }
    }

    template<typename T>
    class StringView
    {
    public:
        constexpr StringView(T* p)
            : m_p(p)
            , m_size(strview_detail::Strlen(p))
        {}

        [[nodiscard]]
        constexpr size_t GetSize() const { return m_size; }

    private:
        T* m_p = nullptr;
        size_t m_size = 0;
    };
}
