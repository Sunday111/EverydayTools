#pragma once

#include "ArrayView.hpp"

namespace edt {
namespace strview_detail {
template <typename T>
[[nodiscard]] constexpr size_t Strlen(const T* p) {
  if (p) {
    size_t i = 0;
    while (p[i] != 0) {
      ++i;
    }

    return i;
  }

  return 0;
}
}  // namespace strview_detail

template <typename T>
class StringViewT {
 public:
  constexpr StringViewT(const T* p)
      : m_p(p), m_size(strview_detail::Strlen(p)) {}

  constexpr StringViewT(const T* p, size_t size) : m_p(p), m_size(size) {}

  [[nodiscard]] constexpr size_t GetSize() const { return m_size; }

  const T* GetData() const { return m_p; }

  constexpr bool operator==(const T* str) const {
    return *this == StringViewT(str);
  }

  constexpr bool operator==(const StringViewT& view) const {
    if (m_size == view.m_size) {
      for (size_t i = 0; i < m_size; ++i) {
        if (m_p[i] != view.m_p[i]) {
          return false;
        }
      }
    }

    return true;
  }

  friend bool operator==(const T* a, const StringViewT& b) { return b == a; }

  friend std::ostream& operator<<(std::ostream& output,
                                  const StringViewT& view) {
    for (size_t i = 0; i < view.GetSize(); ++i) {
      output << view.GetData()[i];
    }

    return output;
  }

 private:
  const T* m_p = nullptr;
  size_t m_size = 0;
};

using StringView = StringViewT<char>;
}  // namespace edt
