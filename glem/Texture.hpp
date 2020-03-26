#pragma once

#include "Bindable.hpp"

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <vector>

namespace glem {

    enum class TextureUsage {
        Texture2D
    };

    template<TextureUsage Usage> struct TextureUsageMap;

    template<> struct TextureUsageMap<TextureUsage::Texture2D> {
        static constexpr GLint usage = GL_TEXTURE_2D;
    };

    class Texture : public Bindable {
    public:
        Texture(const std::vector<uint8_t>& data, const glm::ivec2& size, uint32_t unit, TextureUsage usage = TextureUsage::Texture2D);
        ~Texture() override;

        Texture(Texture&&) = delete;
        Texture(const Texture&) = delete;

        Texture& operator=(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        uint32_t unit() const noexcept {
            return unit_;
        }

    private:
        uint32_t unit_ {0u};

    };

}
