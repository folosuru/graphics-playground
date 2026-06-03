#include <cstddef>
#include <iostream>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

int main() {
  std::ios_base::sync_with_stdio(false);
#ifdef _WIN32
  _setmode(_fileno(stdout), _O_BINARY);
#endif

  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      char pixel[4] = {
          (char)i,
          (char)j,
          (char)128,
      };
      std::cout.write(pixel, 3);
    }
  }
}
