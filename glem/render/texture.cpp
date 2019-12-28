#include "texture.hpp"

#include <glad/glad.h>

#include <tuple>

#include <util/log.hpp>

namespace {
    const std::string TAG = "Texture";

    [[maybe_unused]] std::tuple<GLenum, GLenum> convertFormat(glem::render::Format f) noexcept {
        switch (f) {
        case glem::render::Format::RGB:  { return { GL_RGB,  GL_RGB8  }; }
        case glem::render::Format::RGBA: { return { GL_RGBA, GL_RGBA8 }; }
        default: {
            glem::util::Log::w(TAG, "Format unspecified. Used default RGBA.");
            return { GL_RGBA, GL_RGBA8 };
        }
        }
    }

    [[maybe_unused]] GLenum convertFilter(glem::render::Filter f) noexcept {
        switch (f) {
        case glem::render::Filter::Linear:  { return GL_LINEAR;  }
        case glem::render::Filter::Nearest: { return GL_NEAREST; }
        default: {
            glem::util::Log::w(TAG, "Filter unspecified. Used default Linear.");
            return GL_LINEAR;
        }
        }
    }

    [[maybe_unused]] GLenum convertWrap(glem::render::Wrap w) noexcept {
        switch (w) {
        case glem::render::Wrap::Repeat:         { return GL_REPEAT;          }
        case glem::render::Wrap::ClampToEdge:    { return GL_CLAMP_TO_EDGE;   }
        case glem::render::Wrap::ClampToBorder:  { return GL_CLAMP_TO_BORDER; }
        case glem::render::Wrap::MirroredRepeat: { return GL_MIRRORED_REPEAT; }
        default: {
            glem::util::Log::w(TAG, "Wrap unspecified. Used default Repeat.");
            return GL_REPEAT;
        }
        }
    }
}

namespace glem::render {

    Texture::Texture(const std::string &tag, const std::string &path, Wrap wrap, Filter minFilter, Filter magFilter, uint32_t slot) :
        tag_  {tag},
        slot_ {slot}
    {
        if(auto surface = Surface::load(path)) {
            width_  = surface->width();
            height_ = surface->height();
            format_ = surface->format();

            auto[imageFormat, internalFormat] = convertFormat(format_);

            glCreateTextures(GL_TEXTURE_2D, 1, &id_);

            glTextureStorage2D(id_, 1, internalFormat, width_, height_);

            glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, convertFilter(minFilter));
            glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, convertFilter(magFilter));
            glTextureParameteri(id_, GL_TEXTURE_WRAP_S, convertWrap(wrap));
            glTextureParameteri(id_, GL_TEXTURE_WRAP_T, convertWrap(wrap));

            glTextureSubImage2D(id_, 0, 0, 0, width_, height_, imageFormat, GL_UNSIGNED_BYTE, surface->pixels().data());
        }
        else
            util::Log::e(TAG, "Unable to load image surface.");
    }

    Texture::Texture(const std::string &tag, uint32_t width, uint32_t height, Wrap wrap, Filter minFilter, Filter magFilter, Format format, uint32_t slot) :
        tag_    {tag},
        width_  {width},
        height_ {height},
        slot_   {slot},
        format_ {format}
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &id_);

        auto[imageFormat, internalFormat] = convertFormat(format_);

        glTextureStorage2D(id_, 1, internalFormat, width_, height_);

        glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, convertFilter(minFilter));
        glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, convertFilter(magFilter));
        glTextureParameteri(id_, GL_TEXTURE_WRAP_S, convertWrap(wrap));
        glTextureParameteri(id_, GL_TEXTURE_WRAP_T, convertWrap(wrap));
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &id_);
    }

    void Texture::bind() noexcept
    {
        glBindTextureUnit(slot_, id_);
    }

    std::string Texture::tag() const noexcept
    {
        return tag_;
    }

    uint32_t Texture::width() const noexcept
    {
        return width_;
    }

    uint32_t Texture::height() const noexcept
    {
        return height_;
    }

    bool Texture::setSurface(const Surface &value) noexcept
    {
        if(value.width() != width_ || value.height() != height_ || value.format() != format_) {
            util::Log::e(TAG, "Unable to set surface. Surface must be entire texture.");
            return false;
        }

        auto[imageFormat, internalFormat] = convertFormat(format_);

        glTextureSubImage2D(id_, 0, 0, 0, width_, height_, imageFormat, GL_UNSIGNED_BYTE, value.pixels().data());

        return true;
    }

}
