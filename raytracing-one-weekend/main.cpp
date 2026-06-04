#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>

int main(int argc, char *argv[]) {
  constexpr int image_width = 256;
  constexpr int image_height = 256;

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rremaining " << j << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto r = double(i) / (image_width - 1);
      auto g = double(j) / (image_height - 1);
      auto b = 0.25;
      char ir = static_cast<int>(255.999 * r);
      char ig = static_cast<int>(255.999 * g);
      char ib = static_cast<int>(255.999 * b);
      std::cout << ir << ig << ib;
    }
  }
  std::cerr << '\n';
}
