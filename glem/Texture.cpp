#include "Texture.hpp"

#include "Image.hpp"

#include "Log.hpp"

#include <glad/glad.h>

namespace {
    static constexpr const char* TAG = "Texture";
}

namespace glem {

    template<> struct TextureWrapMap<TextureWrap::Repeat> {
        static constexpr const GLenum wrap = GL_REPEAT;
    };

    template<> struct TextureWrapMap<TextureWrap::MirroredRepeat> {
        static constexpr const GLenum wrap = GL_MIRRORED_REPEAT;
    };

    template<> struct TextureWrapMap<TextureWrap::ClampToEdge> {
        static constexpr const GLenum wrap = GL_CLAMP_TO_EDGE;
    };

    template<> struct TextureWrapMap<TextureWrap::ClampToBorder> {
        static constexpr const GLenum wrap = GL_CLAMP_TO_BORDER;
    };

    template<> struct TextureUsageMap<TextureUsage::Texture2D> {
        static constexpr const GLenum usage = GL_TEXTURE_2D;
    };

    template<> struct TextureUsageMap<TextureUsage::TextureCubemap> {
        static constexpr const GLenum usage = GL_TEXTURE_CUBE_MAP;
    };

    template<> struct TextureFilterMap<TextureFilter::Linear> {
        static constexpr const GLenum filter = GL_LINEAR;
    };

    template<> struct TextureFilterMap<TextureFilter::Nearest> {
        static constexpr const GLenum filter = GL_NEAREST;
    };

    template<> struct TextureFormatMap<TextureFormat::RGB> {
        static constexpr const GLenum format         = GL_RGB;
        static constexpr const GLenum internalFormat = GL_RGB8;
    };

    template<> struct TextureFormatMap<TextureFormat::RGBA> {
        static constexpr const GLenum format         = GL_RGBA;
        static constexpr const GLenum internalFormat = GL_RGBA8;
    };

    Texture::Texture(const Image &data, const TextureSettings &settings) :
        settings_{settings}
    {
        switch (settings_.Usage) {
        case TextureUsage::Texture2D:
            glCreateTextures(TextureUsageMap<TextureUsage::Texture2D>::usage, 1, &handler_);

            bind();

            switch (settings_.InternalFormat) {
            case TextureFormat::RGB:
                glTextureStorage2D(handler_, 1, TextureFormatMap<TextureFormat::RGB>::internalFormat, data.width, data.height);
                break;
            case TextureFormat::RGBA:
                glTextureStorage2D(handler_, 1, TextureFormatMap<TextureFormat::RGBA>::internalFormat, data.width, data.height);
                break;
            }

            switch (settings_.MinFilter) {
            case TextureFilter::Linear:
                glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, TextureFilterMap<TextureFilter::Linear>::filter);
                break;
            case TextureFilter::Nearest:
                glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, TextureFilterMap<TextureFilter::Nearest>::filter);
                break;
            }

            switch (settings_.MagFilter) {
            case TextureFilter::Linear:
                glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, TextureFilterMap<TextureFilter::Linear>::filter);
                break;
            case TextureFilter::Nearest:
                glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, TextureFilterMap<TextureFilter::Nearest>::filter);
                break;
            }

            switch (settings_.WrapSMode) {
            case TextureWrap::Repeat:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::Repeat>::wrap);
                break;
            case TextureWrap::MirroredRepeat:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::MirroredRepeat>::wrap);
                break;
            case TextureWrap::ClampToEdge:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::ClampToEdge>::wrap);
                break;
            case TextureWrap::ClampToBorder:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::ClampToBorder>::wrap);
                break;
            }

            switch (settings_.WrapTMode) {
            case TextureWrap::Repeat:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::Repeat>::wrap);
                break;
            case TextureWrap::MirroredRepeat:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::MirroredRepeat>::wrap);
                break;
            case TextureWrap::ClampToEdge:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::ClampToEdge>::wrap);
                break;
            case TextureWrap::ClampToBorder:
                glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::ClampToBorder>::wrap);
                break;
            }

            switch (settings_.Format) {
            case TextureFormat::RGB:
                glTextureSubImage2D(handler_, 0, 0, 0, data.width, data.height, TextureFormatMap<TextureFormat::RGB>::format, GL_UNSIGNED_BYTE, data.pixels.data());
                break;
            case TextureFormat::RGBA:
                glTextureSubImage2D(handler_, 0, 0, 0, data.width, data.height, TextureFormatMap<TextureFormat::RGBA>::format, GL_UNSIGNED_BYTE, data.pixels.data());
                break;
            }

            unbind();

            break;
        default:
            Log::e(TAG, "Unsupported texture usage.");
            break;
        }
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &handler_);
    }

    void Texture::bind() const noexcept
    {
        glBindTextureUnit(settings_.Unit, handler_);
    }

    void Texture::unbind() const noexcept
    {
        glBindTextureUnit(settings_.Unit, 0);
    }

    const TextureSettings &Texture::settings() const noexcept
    {
        return settings_;
    }

    Cubemap::Cubemap(const std::array<Image, 6> &data, const TextureSettings &settings) :
        settings_{settings}
    {
        glCreateTextures(TextureUsageMap<TextureUsage::TextureCubemap>::usage, 1, &handler_);

        bind();

        for(size_t i = 0; i < data.size(); ++i) {
            GLenum format;
            GLenum internalFormat;

            switch (settings_.Format) {
            case TextureFormat::RGB:
                format = TextureFormatMap<TextureFormat::RGB>::format;
                break;
            case TextureFormat::RGBA:
                format = TextureFormatMap<TextureFormat::RGBA>::format;
                break;
            }

            switch (settings_.InternalFormat) {
            case TextureFormat::RGB:
                internalFormat = TextureFormatMap<TextureFormat::RGB>::internalFormat;
                break;
            case TextureFormat::RGBA:
                internalFormat = TextureFormatMap<TextureFormat::RGBA>::internalFormat;
                break;
            }

            glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, internalFormat, data[i].width, data[i].height, 0, format, GL_UNSIGNED_BYTE, data[i].pixels.data());
        }

        switch (settings_.MinFilter) {
        case TextureFilter::Linear:
            glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, TextureFilterMap<TextureFilter::Linear>::filter);
            break;
        case TextureFilter::Nearest:
            glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, TextureFilterMap<TextureFilter::Nearest>::filter);
            break;
        }

        switch (settings_.MagFilter) {
        case TextureFilter::Linear:
            glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, TextureFilterMap<TextureFilter::Linear>::filter);
            break;
        case TextureFilter::Nearest:
            glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, TextureFilterMap<TextureFilter::Nearest>::filter);
            break;
        }

        switch (settings_.WrapSMode) {
        case TextureWrap::Repeat:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::Repeat>::wrap);
            break;
        case TextureWrap::MirroredRepeat:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::MirroredRepeat>::wrap);
            break;
        case TextureWrap::ClampToEdge:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::ClampToEdge>::wrap);
            break;
        case TextureWrap::ClampToBorder:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, TextureWrapMap<TextureWrap::ClampToBorder>::wrap);
            break;
        }

        switch (settings_.WrapTMode) {
        case TextureWrap::Repeat:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::Repeat>::wrap);
            break;
        case TextureWrap::MirroredRepeat:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::MirroredRepeat>::wrap);
            break;
        case TextureWrap::ClampToEdge:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::ClampToEdge>::wrap);
            break;
        case TextureWrap::ClampToBorder:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, TextureWrapMap<TextureWrap::ClampToBorder>::wrap);
            break;
        }

        switch (settings_.WrapRMode) {
        case TextureWrap::Repeat:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_R, TextureWrapMap<TextureWrap::Repeat>::wrap);
            break;
        case TextureWrap::MirroredRepeat:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_R, TextureWrapMap<TextureWrap::MirroredRepeat>::wrap);
            break;
        case TextureWrap::ClampToEdge:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_R, TextureWrapMap<TextureWrap::ClampToEdge>::wrap);
            break;
        case TextureWrap::ClampToBorder:
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_R, TextureWrapMap<TextureWrap::ClampToBorder>::wrap);
            break;
        }

        unbind();
    }

    Cubemap::~Cubemap()
    {
        glDeleteTextures(1, &handler_);
    }

    void Cubemap::bind() const noexcept
    {
        glBindTextureUnit(settings_.Unit, handler_);
    }

    void Cubemap::unbind() const noexcept
    {
        glBindTextureUnit(settings_.Unit, 0);
    }

    const TextureSettings &Cubemap::settings() const noexcept
    {
        return settings_;
    }

}
