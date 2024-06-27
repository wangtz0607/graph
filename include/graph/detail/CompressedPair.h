#pragma once

#include <type_traits>
#include <utility>

namespace graph::detail {

template <typename T1, typename T2>
class CompressedPair {
public:
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type &;
    using second_reference = second_type &;
    using first_const_reference = const first_type &;
    using second_const_reference = const second_type &;

    CompressedPair() = default;

    CompressedPair(first_type first, second_type second) : first_(std::move(first)), second_(std::move(second)) {}

    first_reference first() {
        return first_;
    }

    first_const_reference first() const {
        return first_;
    }

    second_reference second() {
        return second_;
    }

    second_const_reference second() const {
        return second_;
    }

private:
    first_type first_;
    second_type second_;
};

template <typename T1, typename T2>
    requires std::is_empty_v<T1> && (!std::is_final_v<T1>)
class CompressedPair<T1, T2> : private T1 {
public:
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type &;
    using second_reference = second_type &;
    using first_const_reference = const first_type &;
    using second_const_reference = const second_type &;

    CompressedPair() = default;

    CompressedPair(first_type, second_type second) : second_(std::move(second)) {}

    first_reference first() {
        return *this;
    }

    first_const_reference first() const {
        return *this;
    }

    second_reference second() {
        return second_;
    }

    second_const_reference second() const {
        return second_;
    }

private:
    second_type second_;
};

template <typename T1, typename T2>
    requires std::is_empty_v<T2> && (!std::is_final_v<T2>)
class CompressedPair<T1, T2> : private T2 {
public:
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type &;
    using second_reference = second_type &;
    using first_const_reference = const first_type &;
    using second_const_reference = const second_type &;

    CompressedPair() = default;

    CompressedPair(first_type first, second_type) : first_(std::move(first)) {}

    first_reference first() {
        return first_;
    }

    first_const_reference first() const {
        return first_;
    }

    second_reference second() {
        return *this;
    }

    second_const_reference second() const {
        return *this;
    }

private:
    first_type first_;
};

} // namespace graph::detail
