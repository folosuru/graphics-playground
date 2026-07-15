#ifndef INCLUDE_SRC_RANGE_HPP_
#define INCLUDE_SRC_RANGE_HPP_

#include <algorithm>

#include "math_util.hpp"

class range {
public:
    range(RealType min, RealType max) : min_(min), max_(max) {}

    static range create(RealType v1, RealType v2) {
        return range(std::min(v1, v2), std::max(v1, v2));
    }

    bool contains(RealType value) const {
        return min_ <= value && value <= max_;
    }

    bool contains_strict(RealType value) const {
        return min_ < value && value < max_;
    }

    RealType clamp(RealType value) { return std::clamp(value, min_, max_); }

    range And(const range& other) const {
        return {std::max(this->min_, other.min_),
                std::min(this->max_, other.max_)};
    }

    range Or(const range& other) const {
        return {std::min(this->min_, other.min_),
                std::max(this->max_, other.max_)};
    }

    bool valid() const { return (min_ <= max_); }

    RealType min() const { return min_; }
    RealType max() const { return max_; }

    static range Everything;

private:
    RealType min_;
    RealType max_;
};

range range::Everything = {-Infinity, Infinity};

#endif  // INCLUDE_SRC_RANGE_HPP_
