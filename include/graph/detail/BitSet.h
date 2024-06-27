#pragma once

#include <bitset>
#include <cstddef>
#include <type_traits>

namespace graph::detail {

template <typename>
struct is_std_bitset_impl : std::false_type {};

template <size_t N>
struct is_std_bitset_impl<std::bitset<N>> : std::true_type {};

template <typename T>
struct is_std_bitset : is_std_bitset_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_bitset_v = is_std_bitset<T>::value;

template <typename T>
concept BitSet = is_std_bitset_v<T>;

} // namespace detail
