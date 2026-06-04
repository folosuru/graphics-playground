#ifndef COLOR_HPP_
#define COLOR_HPP_
#include <iostream>
#include <ostream>

#include "vec3.hpp"

void write_color(std::ostream& os, Color c) {
    os << static_cast<char>(255.999 * c.x())
       << static_cast<char>(255.999 * c.y())
       << static_cast<char>(255.999 * c.z());
}

#endif  // COLOR_HPP_
