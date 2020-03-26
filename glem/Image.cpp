#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __WIN32__
    #define STBI_MSC_SECURE_CRT
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace glem {

    Image::Image(int w, int h, const std::vector<uint8_t> &p) :
        width{w}, height{h}, pixels{p}
    {

    }

    Image Image::load(const std::string &filepath, bool flip) noexcept
    {
        int w {0};
        int h {0};
        int c {0};

        stbi_set_flip_vertically_on_load(flip);

        auto data = stbi_load(filepath.c_str(), &w, &h, &c, STBI_default);

        std::vector<uint8_t> p{data, data + w * h * c};

        stbi_image_free(data);

        return Image{w, h, p};
    }

}
