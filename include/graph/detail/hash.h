#pragma once

#include <cstddef>
#include <functional>

namespace graph::detail {

inline constexpr size_t combineHashes(size_t hash1, size_t hash2) {
    return (hash1 << 1) ^ hash2;
}

template <typename T>
constexpr size_t hashValue(const T &value) {
    return std::hash<T>()(value);
}

template <typename T>
constexpr size_t hashValues(const T &value) {
    return hashValue(value);
}

template <typename T, typename... Ts>
constexpr size_t hashValues(const T &value, const Ts &...values) {
    return combineHashes(hashValue(value), hashValues(values...));
}

} // namespace graph
