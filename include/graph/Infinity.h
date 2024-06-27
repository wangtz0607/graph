#pragma once

#include <limits>

namespace graph {

template <typename T>
inline constexpr T kInfinity = std::numeric_limits<T>::infinity();

template <typename T>
    requires (!std::numeric_limits<T>::has_infinity)
inline constexpr T kInfinity<T> = std::numeric_limits<T>::max();

struct Infinity {
    template <typename T>
    constexpr operator T() const {
        return kInfinity<T>;
    }
};

} // namespace graph
