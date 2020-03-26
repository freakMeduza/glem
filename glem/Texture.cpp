#include "Texture.hpp"

namespace glem {

    Texture::Texture(const std::vector<uint8_t> &data, const glm::ivec2 &size, uint32_t unit, TextureUsage usage) :
        unit_{unit}
    {
        switch (usage) {
        case TextureUsage::Texture2D:
            glCreateTextures(TextureUsageMap<TextureUsage::Texture2D>::usage, 1, &handler_);
            break;
        }

        // TODO: format and other shit
        glTextureStorage2D(handler_, 1, GL_RGBA8, size.x, size.y);

        glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureSubImage2D(handler_, 0, 0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &handler_);
    }

    void Texture::bind() const noexcept
    {
        glBindTextureUnit(unit_, handler_);
    }

    void Texture::unbind() const noexcept
    {
        glBindTextureUnit(unit_, 0);
    }

}
