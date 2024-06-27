#pragma once

#include <iterator>

namespace graph::detail {

template <typename M>
class MapKeyIterator {
public:
    using iterator_category = std::iterator_traits<typename M::const_iterator>::iterator_category;
    using value_type = M::key_type;
    using difference_type = std::iterator_traits<typename M::const_iterator>::difference_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    MapKeyIterator() = default;

    explicit MapKeyIterator(M::const_iterator base) : base_(base) {}

    M::const_iterator base() const {
        return base_;
    }

    pointer operator->() const {
        return &base_->first;
    }

    reference operator*() const {
        return base_->first;
    }

    MapKeyIterator &operator++() {
        ++base_;
        return *this;
    }

    MapKeyIterator operator++(int) {
        return MapKeyIterator(base_++);
    }

    MapKeyIterator &operator--() {
        --base_;
        return *this;
    }

    MapKeyIterator operator--(int) {
        return MapKeyIterator(base_--);
    }

    friend bool operator==(MapKeyIterator lhs, MapKeyIterator rhs) {
        return lhs.base_ == rhs.base_;
    }

private:
    M::const_iterator base_;
};

} // namespace graph::detail
