#ifndef INCLUDE_IMAGE_IMAGE_FILE_HPP_
#define INCLUDE_IMAGE_IMAGE_FILE_HPP_

#include <algorithm>
#include <cstddef>
#include <string>

#include "../vec2.hpp"
#include "../vec3.hpp"

class image_file {
public:
    static image_file *create(const std::string& str);
    ~image_file();

    Color get_pixel(int x, int y) const {
        auto x_ = std::clamp(x, 0, width - 1);
        auto y_ = std::clamp(y, 0, height - 1);
        auto result = Vec3::from_unalign_array(
            data + ((width * y_ + x_) * elem_per_pixel));
        return result;
    }

    Color get_pixel(float x, float y) const {
        [[assume(0 <= x && x <= 1)]];
        [[assume(0 <= y && y <= 1)]];

        return get_pixel(static_cast<int>(x * width),
                         static_cast<int>(y * height));
    }

    Color get_pixel(Vec2 xy) const { return get_pixel(xy[0], xy[1]); }

private:
    int width, height;

    float *data = nullptr;
    static constexpr size_t elem_per_pixel = 3;

    image_file() = default;
    image_file(const image_file&) = default;
    image_file& operator=(const image_file&) = default;
};

#endif  // INCLUDE_IMAGE_IMAGE_FILE_HPP_
