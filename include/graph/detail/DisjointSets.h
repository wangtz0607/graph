#pragma once

#include <cstddef>
#include <unordered_map>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph::detail {

template <typename Key,
          typename Parents = std::unordered_map<Key, Key>,
          typename Ranks = std::unordered_map<Key, size_t>>
class DisjointSets {
public:
    void makeSet(Key x) {
        put(parents_, x, x);
        put(ranks_, x, 0);
    }

    Key find(Key x) {
        if (get(parents_, x) == x) {
            return x;
        }
        put(parents_, x, find(get(parents_, x)));
        return get(parents_, x);
    }

    void union_(Key x, Key y) {
        using std::swap;

        x = find(x);
        y = find(y);
        if (x != y) {
            if (get(ranks_, x) > get(ranks_, y)) {
                swap(x, y);
            }
            put(parents_, x, y);
            if (get(ranks_, x) == get(ranks_, y)) {
                put(ranks_, y, get(ranks_, x) + 1);
            }
        }
    }

private:
    mutable Parents parents_;
    mutable Ranks ranks_;
};

} // namespace graph
