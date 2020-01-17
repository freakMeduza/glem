#include "surface.hpp"

#include <util/log.hpp>

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

    const std::string TAG = "Surface";
}

namespace glem::render {

    Surface::Surface(const std::vector<uint8_t> &pixels, uint32_t width, uint32_t height, Format format) :
        pixels_ {pixels},
        width_  {width},
        height_ {height},
        format_ {format}
    {

    }

    std::shared_ptr<Surface> Surface::load(const std::string &path, const Options &options) noexcept
    {
        int width      {0};
        int height     {0};
        int components {0};

        stbi_set_flip_vertically_on_load(options.verticalFlip ? 1 : 0);

        auto data = stbi_load(path.c_str(), &width, &height, &components, STBI_default);

        if(!data) {
            util::Log::e(TAG, "Unable to load image.");
            return {};
        }

        Format format {Format::Unspecified};

        if(components == 1)
            format = Format::R;
        else if(components == 3)
            format = Format::RGB;
        else if(components == 4)
            format = Format::RGBA;

        if(format == Format::Unspecified) {
            util::Log::e(TAG, "Unsupported image format.");
            return {};
        }

        std::vector<uint8_t> pixels{data, data + width * height * components};

        stbi_image_free(data);

        return std::make_shared<Surface>(pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height), format);
    }

    bool Surface::save(const std::string &path) const noexcept
    {
        if(format_ == Format::R) {
            auto _path = substring(path).append(SUFFIX_JPG);

            int ret = stbi_write_jpg(_path.c_str(), width_, height_, 1, pixels_.data(), 100);

            return (ret != 0);
        }
        else if(format_ == Format::RGB) {
            auto _path = substring(path).append(SUFFIX_JPG);

            int ret = stbi_write_jpg(_path.c_str(), width_, height_, 3, pixels_.data(), 100);

            return (ret != 0);
        }
        else if(format_ == Format::RGBA) {
            auto _path = substring(path).append(SUFFIX_PNG);

            int ret = stbi_write_png(_path.c_str(), width_, height_, 4, pixels_.data(), width_ * 4);

            return (ret != 0);
        }

        util::Log::e(TAG, "Unspecified image format.");

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
