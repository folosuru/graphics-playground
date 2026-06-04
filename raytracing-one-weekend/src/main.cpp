#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>

#include "color.hpp"

int main(int argc, char *argv[]) {
    constexpr int image_width = 256;
    constexpr int image_height = 256;

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rremaining " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            write_color(std::cout,
                        Vec3{((double)i / (image_width - 1)),
                             ((double)j / (image_height - 1)), 0.25});
        }
    }
    std::cerr << '\n';
}
