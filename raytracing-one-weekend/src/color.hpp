#ifndef COLOR_HPP_
#define COLOR_HPP_
#include <algorithm>
#include <iostream>
#include <ostream>

#include "vec3.hpp"

void write_color(std::ostream& os, Color c) {
    os << static_cast<char>(255.999 * c.x())
       << static_cast<char>(255.999 * c.y())
       << static_cast<char>(255.999 * c.z());
}

void write_color(std::ostream& os, Color c, int samples) {
    using std::clamp;
    c /= samples;

    os << static_cast<char>(255.999 * clamp(sqrt(c.x()), 0.0, 0.999))
       << static_cast<char>(255.999 * clamp(sqrt(c.y()), 0.0, 0.999))
       << static_cast<char>(255.999 * clamp(sqrt(c.z()), 0.0, 0.999));
}

void write_colorbuf(char *pos, Color c, int samples) {
    using std::clamp;
    c /= samples;

    pos[0] = static_cast<char>(255.999 * clamp(sqrt(c.x()), 0.0, 0.999));
    pos[1] = static_cast<char>(255.999 * clamp(sqrt(c.y()), 0.0, 0.999));
    pos[2] = static_cast<char>(255.999 * clamp(sqrt(c.z()), 0.0, 0.999));
}
#endif  // COLOR_HPP_
