#pragma once

#include <cstddef>

#include "graph/detail/BitSet.h"
#include "graph/detail/Map.h"
#include "graph/detail/RandomAccessSequence.h"
#include "graph/detail/Set.h"

namespace graph {

template <typename T>
struct GeneralizedMapTraits {
    using Key = T::Key;
    using Value = T::Value;
};

template <typename M>
    requires detail::Map<M>
struct GeneralizedMapTraits<M> {
    using Key = M::key_type;
    using Value = M::mapped_type;
};

template <typename M>
    requires detail::Map<M>
struct GeneralizedMapTraits<M *> : GeneralizedMapTraits<M> {};

template <typename R>
    requires detail::RandomAccessSequence<R>
struct GeneralizedMapTraits<R> {
    using Key = R::size_type;
    using Value = R::value_type;
};

template <typename R>
    requires detail::RandomAccessSequence<R>
struct GeneralizedMapTraits<R *> : GeneralizedMapTraits<R> {};

template <typename B>
    requires detail::BitSet<B>
struct GeneralizedMapTraits<B> {
    using Key = size_t;
    using Value = bool;
};

template <typename B>
    requires detail::BitSet<B>
struct GeneralizedMapTraits<B *> : GeneralizedMapTraits<B> {};

template <typename S>
    requires detail::Set<S>
struct GeneralizedMapTraits<S> {
    using Key = S::key_type;
    using Value = bool;
};

template <typename S>
    requires detail::Set<S>
struct GeneralizedMapTraits<S *> : GeneralizedMapTraits<S> {};

template <typename M>
    requires detail::Map<M>
bool contains(M &m, const typename M::key_type &key) {
    return m.contains(key);
}

template <typename M>
    requires detail::Map<M>
bool contains(M *m, const typename M::key_type &key) {
    return contains(*m, key);
}

template <typename M>
    requires detail::Map<M>
decltype(auto) get(M &m, const typename M::key_type &key) {
    return (m.find(key)->second);
}

template <typename M>
    requires detail::Map<M>
decltype(auto) get(M *m, const typename M::key_type &key) {
    return get(*m, key);
}

template <typename M>
    requires detail::Map<M>
void put(M &m, typename M::key_type key, typename M::mapped_type value) {
    m.insert_or_assign(std::move(key), std::move(value));
}

template <typename M>
    requires detail::Map<M>
void put(M *m, typename M::key_type key, typename M::mapped_type value) {
    put(*m, std::move(key), std::move(value));
}

template <typename M>
    requires detail::Map<M>
void remove(M &m, const typename M::key_type &key) {
    m.erase(key);
}

template <typename M>
    requires detail::Map<M>
void remove(M *m, const typename M::key_type &key) {
    remove(*m, key);
}

template <typename R>
    requires detail::RandomAccessSequence<R>
decltype(auto) get(R &r, typename R::size_type key) {
    return (r[key]);
}

template <typename R>
    requires detail::RandomAccessSequence<R>
decltype(auto) get(R *r, typename R::size_type key) {
    return get(*r, key);
}

template <typename R>
    requires detail::RandomAccessSequence<R>
void put(R &r, typename R::size_type key, typename R::value_type value) {
    r[key] = std::move(value);
}

template <typename R>
    requires detail::RandomAccessSequence<R>
void put(R *r, typename R::size_type key, typename R::value_type value) {
    put(*r, key, std::move(value));
}

template <typename B>
    requires detail::BitSet<B>
bool get(B &b, size_t key) {
    return b.test(key);
}

template <typename B>
    requires detail::BitSet<B>
bool get(B *b, size_t key) {
    return get(*b, key);
}

template <typename B>
    requires detail::BitSet<B>
void put(B &b, size_t key, bool value) {
    b.set(key, value);
}

template <typename B>
    requires detail::BitSet<B>
void put(B *b, size_t key, bool value) {
    put(*b, key, value);
}

template <typename S>
    requires detail::Set<S>
bool get(S &s, const typename S::key_type &key) {
    return s.contains(key);
}

template <typename S>
    requires detail::Set<S>
bool get(S *s, const typename S::key_type &key) {
    return get(*s, key);
}

template <typename S>
    requires detail::Set<S>
void put(S &s, const typename S::key_type &key, bool value) {
    if (value) {
        s.insert(key);
    } else {
        s.erase(key);
    }
}

template <typename S>
    requires detail::Set<S>
void put(S *s, const typename S::key_type &key, bool value) {
    put(*s, key, value);
}

} // namespace graph
