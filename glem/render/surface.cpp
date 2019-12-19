#include "surface.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace {
    [[maybe_unused]] std::string substring(const std::string& str) noexcept {
        auto _str = str;
        auto index = _str.find_last_of(".");

        if(index != std::string::npos)
            _str = _str.substr(0, index);

        return _str;
    }

    const std::string SUFFIX_JPG = ".jpg";
    const std::string SUFFIX_PNG = ".png";
}

namespace glem::render {

    Surface::Surface(const std::vector<uint8_t> &pixels, uint32_t width, uint32_t height, Format format) :
        pixels_ {pixels},
        width_  {width},
        height_ {height},
        format_ {format}
    {

    }

    std::optional<Surface> Surface::load(const std::string &path) noexcept
    {
        int width      {0};
        int height     {0};
        int components {0};

        stbi_set_flip_vertically_on_load(1);

        auto data = stbi_load(path.c_str(), &width, &height, &components, STBI_default);

        if(!data) {
            std::cerr << "Unable to load image." << std::endl;
            return {};
        }

        std::cout << "Image loaded ( " << width << "x" << height << " [" << components << "] )" << std::endl;

        Format format {Format::Unspecified};

        if(components == 3)
            format = Format::RGB;
        else if(components == 4)
            format = Format::RGBA;

        if(format == Format::Unspecified) {
            std::cerr << "Unsupported image format." << std::endl;
            return {};
        }

        std::vector<uint8_t> pixels{data, data + width * height * components};

        stbi_image_free(data);

        return Surface{pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height), format};
    }

    bool Surface::save(const std::string &path) const noexcept
    {
        if(format_ == Format::RGB) {
            std::cout << "RGB image format. Save as jpg." << std::endl;

            auto _path = substring(path).append(SUFFIX_JPG);

            std::cout << "Full path " << _path << std::endl;

            int ret = stbi_write_jpg(_path.c_str(), width_, height_, 3, pixels_.data(), 100);

            return (ret != 0);
        }
        else if(format_ == Format::RGBA) {
            std::cout << "RGBA image format. Save as png." << std::endl;

            auto _path = substring(path).append(SUFFIX_PNG);

            std::cout << "Full path " << _path << std::endl;

            int ret = stbi_write_png(_path.c_str(), width_, height_, 4, pixels_.data(), width_ * 4);

            return (ret != 0);
        }

        std::cerr << "Unspecified image format." <<std::endl;

        return false;
    }

    uint32_t Surface::width() const noexcept
    {
        return width_;
    }

    uint32_t Surface::height() const noexcept
    {
        return height_;
    }

    Format Surface::format() const noexcept
    {
        return format_;
    }

    const std::vector<uint8_t> &Surface::pixels() const noexcept
    {
        return pixels_;
    }

}
