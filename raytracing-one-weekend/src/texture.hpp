#ifndef INCLUDE_SRC_TEXTURE_HPP_
#define INCLUDE_SRC_TEXTURE_HPP_
#include "vec2.hpp"
#include "vec3.hpp"

class texture {
public:
    virtual Color get(Vec2) const = 0;
};

class checker_texture : public texture {
public:
    Color get(Vec2 v) const override {
        if (static_cast<int>(v[0]) & 0x1) {
            return Color{0.2, 0.3, 0.1};
        } else {
            return Color{1, 1, 1};
        }
    }
};

class test_texture : public texture {
    Color get(Vec2 v) const override { return Color{v[0], 0.5, v[1]}; }
};

#endif  // INCLUDE_SRC_TEXTURE_HPP_
