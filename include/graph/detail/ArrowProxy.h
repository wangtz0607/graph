#pragma once

#include <utility>

namespace graph::detail {

template <typename T>
class ArrowProxy {
public:
    explicit ArrowProxy(T value)
        : value_(std::move(value)) {}

    template <typename... Args>
    explicit ArrowProxy(std::in_place_t, Args &&...args)
        : value_(std::forward<Args>(args)...) {}

    T *operator->() {
        return &value_;
    }

    T *operator->() const {
        return &value_;
    }

    template <typename... Args>
    static ArrowProxy<T> make(Args &&...args) {
        return ArrowProxy<T>(std::in_place, std::forward<Args>(args)...);
    }

private:
    T value_;
};

} // namespace graph::detail
