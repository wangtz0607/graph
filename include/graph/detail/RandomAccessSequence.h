#pragma once

#include <array>
#include <cstddef>
#include <deque>
#include <type_traits>
#include <vector>

namespace graph::detail {

template <typename>
struct is_std_array_impl : std::false_type {};

template <typename T, size_t N>
struct is_std_array_impl<std::array<T, N>> : std::true_type {};

template <typename>
struct is_std_vector_impl : std::false_type {};

template <typename T, typename Allocator>
struct is_std_vector_impl<std::vector<T, Allocator>> : std::true_type {};

template <typename>
struct is_std_deque_impl : std::false_type {};

template <typename T, typename Allocator>
struct is_std_deque_impl<std::deque<T, Allocator>> : std::true_type {};

template <typename T>
struct is_std_array : is_std_array_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_array_v = is_std_array<T>::value;

template <typename T>
struct is_std_vector : is_std_vector_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_vector_v = is_std_vector<std::remove_cv_t<T>>::value;

template <typename T>
struct is_std_deque : is_std_deque_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_deque_v = is_std_deque<T>::value;

template <typename T>
concept RandomAccessSequence = is_std_array_v<T> || is_std_vector_v<T> || is_std_deque_v<T>;

} // namespace graph::detail
