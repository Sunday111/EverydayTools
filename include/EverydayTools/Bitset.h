#pragma once

#include <bitset>
#include <vector>

namespace edt
{
    class Bitset
    {
    public:
        static constexpr auto partSizeBytes = sizeof(void*);
        static constexpr auto partSizeBits = partSizeBytes * 8;
        using Part = std::bitset<partSizeBits>;

        Bitset(size_t size) :
            m_size(size) {
            m_parts.resize(m_size / partSizeBits + 1);
        }

        void Set(size_t index, bool value = true) {
            assert(index < m_size);
            auto partIndex = index / partSizeBits;
            auto indexInPart = index % partSizeBits;
            m_parts[partIndex].set(indexInPart, value);
        }

        bool IsSet(size_t index) const {
            assert(index < m_size);
            auto partIndex = index / partSizeBits;
            auto indexInPart = index % partSizeBits;
            return m_parts[partIndex].test(indexInPart);
        }

        bool All() const {
            if (m_parts.size() > 1) {
                for (size_t i = 0; i < m_parts.size() - 1; ++i) {
                    if (!m_parts[i].all()) {
                        return false;
                    }
                }
            }

            size_t tail = m_size % partSizeBits;
            if (tail == 0 && !m_parts.back().any()) {
                return false;
            }

            auto actualValue = m_parts.back().to_ullong();
            unsigned long long expectedValue = 0;
            expectedValue = ~expectedValue;
            expectedValue >>= partSizeBits - tail;
            return actualValue == expectedValue;
        }

    private:
        size_t m_size = 0;
        std::vector<Part> m_parts;
    };
}
