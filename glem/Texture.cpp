#include "Texture.hpp"

// TODO: do something with this dirty shit!
#include "Image.hpp"

#include "Log.hpp"

namespace {
    static constexpr const char* TAG = "Texture";
}

namespace glem {

    Texture::Texture(const std::vector<uint8_t> &data, const glm::ivec2 &size, uint32_t unit, TextureUsage usage) :
        unit_{unit}
    {
        switch (usage) {
        case TextureUsage::Texture2D: {
            glCreateTextures(TextureUsageMap<TextureUsage::Texture2D>::usage, 1, &handler_);

            glBindTexture(TextureUsageMap<TextureUsage::Texture2D>::usage, handler_);

            glTextureStorage2D(handler_, 1, GL_RGBA8, size.x, size.y);

            glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTextureSubImage2D(handler_, 0, 0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

            glBindTexture(TextureUsageMap<TextureUsage::Texture2D>::usage, 0);
            break;
        }
        case TextureUsage::TextureCubeMap: {
            glCreateTextures(TextureUsageMap<TextureUsage::TextureCubeMap>::usage, 1, &handler_);

            glBindTexture(TextureUsageMap<TextureUsage::TextureCubeMap>::usage, handler_);

            std::vector<const char*> face {
                "skybox/posx.jpg",
                "skybox/negx.jpg",
                "skybox/posy.jpg",
                "skybox/negy.jpg",
                "skybox/posz.jpg",
                "skybox/negz.jpg"
            };

            for(size_t i = 0; i < face.size(); ++i) {
                auto image = Image::load(face[i], false);

                if(!image.pixels.empty())
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.pixels.data());
                else
                    Log::e(TAG, "Failed to load image: ", face[i]);
            }

            glTextureParameteri(handler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(handler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureParameteri(handler_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(TextureUsageMap<TextureUsage::TextureCubeMap>::usage, 0);

            break;
        }
        }

        // TODO: format and other shit
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
