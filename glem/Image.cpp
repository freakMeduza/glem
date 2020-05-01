#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#ifdef __WIN32__
    #define STBI_MSC_SECURE_CRT
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Log.hpp"

namespace {
    static constexpr const char* TAG = "Image";
}

namespace glem {

    Image::Image(int width, int height, int channels, const std::vector<uint8_t> &data) :
        width_{width}, height_{height}, channels_{channels}, data_{data}
    {

    }

    std::optional<Image> Image::load(const std::string &filepath, bool flip) noexcept
    {
        int w {0};
        int h {0};
        int c {0};

        stbi_set_flip_vertically_on_load(flip);

        auto data = stbi_load(filepath.c_str(), &w, &h, &c, STBI_default);

        if(!data)
            return {};

        std::vector<uint8_t> p{data, data + w * h * c};

        stbi_image_free(data);

        return Image{w, h, c, p};
    }

    bool Image::save(const Image &image, const std::string &filepath) noexcept
    {
        if(image.channels() == 3) {
            int ret = stbi_write_jpg(filepath.c_str(), image.width(), image.height(), image.channels(), image.data().data(), 100);

            return (ret != 0);
        }
        else if(image.channels() == 4) {
            int ret = stbi_write_png(filepath.c_str(), image.width(), image.height(), image.channels(), image.data().data(), image.width() * image.channels());

            return (ret != 0);
        }

        Log::e(TAG, "Failed to save image. Unsupported image format ( channels: ", image.channels(), ")");

        return false;
    }

    int Image::width() const noexcept
    {
        return width_;
    }

    int Image::height() const noexcept
    {
        return height_;
    }

    int Image::channels() const noexcept
    {
        return channels_;
    }

    const std::vector<uint8_t> &Image::data() const noexcept
    {
        return data_;
    }

}
