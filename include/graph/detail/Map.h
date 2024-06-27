#pragma once

#include <map>
#include <type_traits>
#include <unordered_map>

namespace graph::detail {

template <typename>
struct is_std_map_impl : std::false_type {};

template <typename Key, typename T, typename Compare, typename Allocator>
struct is_std_map_impl<std::map<Key, T, Compare, Allocator>> : std::true_type {};

template <typename>
struct is_std_unordered_map_impl : std::false_type {};

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_std_unordered_map_impl<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>> : std::true_type {};

template <typename T>
struct is_std_map : is_std_map_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_map_v = is_std_map<T>::value;

template <typename T>
struct is_std_unordered_map : is_std_unordered_map_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_unordered_map_v = is_std_unordered_map<T>::value;

template <typename T>
concept Map = is_std_map_v<T> || is_std_unordered_map_v<T>;

} // namespace graph::detail
