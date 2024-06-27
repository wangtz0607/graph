#pragma once

#include <set>
#include <type_traits>
#include <unordered_set>

namespace graph::detail {

template <typename>
struct is_std_set_impl : std::false_type {};

template <typename Key, typename Compare, typename Allocator>
struct is_std_set_impl<std::set<Key, Compare, Allocator>> : std::true_type {};

template <typename>
struct is_std_unordered_set_impl : std::false_type {};

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct is_std_unordered_set_impl<std::unordered_set<Key, Hash, KeyEqual, Allocator>> : std::true_type {};

template <typename T>
struct is_std_set : is_std_set_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_set_v = is_std_set<T>::value;

template <typename T>
struct is_std_unordered_set : is_std_unordered_set_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_unordered_set_v = is_std_unordered_set<T>::value;

template <typename T>
concept Set = is_std_set_v<T> || is_std_unordered_set_v<T>;

} // namespace graph::detail
