#pragma once

#include <type_traits>

namespace graph {

template <typename K, typename V>
struct Sink {
    using Key = K;
    using Value = V;
};

template <typename K, typename V>
void put(const Sink<K, V> &, const std::type_identity_t<K> &, const std::type_identity_t<V> &) {}

template <typename K, typename V>
void remove(const Sink<K, V> &, const std::type_identity_t<K> &) {}

} // namespace graph
