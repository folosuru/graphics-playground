#include "image_file.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../thiredparty/stb/stb_image.h"

image_file *image_file::create(const std::string& str) {
    image_file result;
    int n = 3;
    result.data = stbi_loadf(str.c_str(), &result.width, &result.height, &n,
                             elem_per_pixel);
    if (result.data == nullptr) {
        return nullptr;
    }
    auto result_ptr = new image_file(result);
    result.data = nullptr;
    return result_ptr;
}

image_file::~image_file() {
    if (data != nullptr) {
        STBI_FREE(data);
    }
}
