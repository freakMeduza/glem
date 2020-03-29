#pragma once

#include "Bindable.hpp"

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace glem {

    class Image;

    enum class TextureWrap {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class TextureUsage {
        Texture2D,
        TextureCubemap
    };

    enum class TextureFilter {
        Linear,
        Nearest
    };

    enum class TextureFormat {
        RGB,
        RGBA
    };

    template<TextureWrap>   struct TextureWrapMap;
    template<TextureUsage>  struct TextureUsageMap;
    template<TextureFilter> struct TextureFilterMap;
    template<TextureFormat> struct TextureFormatMap;

    struct TextureSettings {
        TextureWrap WrapSMode {TextureWrap::Repeat};
        TextureWrap WrapTMode {TextureWrap::Repeat};
        TextureWrap WrapRMode {TextureWrap::Repeat};

        TextureUsage Usage {TextureUsage::Texture2D};

        TextureFilter MinFilter {TextureFilter::Linear};
        TextureFilter MagFilter {TextureFilter::Linear};

        TextureFormat Format         {TextureFormat::RGBA};
        TextureFormat InternalFormat {TextureFormat::RGBA};

        int Unit = 0;
    };

    class Texture : public Bindable {
    public:
        Texture(const Image& data, const TextureSettings& settings);
        ~Texture() override;

        Texture(Texture&&) = delete;
        Texture(const Texture&) = delete;

        Texture& operator=(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief Texture settings
         * @return
         */
        const TextureSettings& settings() const noexcept;

    private:
        TextureSettings settings_;

    };

    class Cubemap : public Bindable {
    public:
        Cubemap(const std::array<Image, 6>& data, const TextureSettings& settings);
        ~Cubemap() override;

        Cubemap(Cubemap&&) = delete;
        Cubemap(const Cubemap&) = delete;

        Cubemap& operator=(Cubemap&&) = delete;
        Cubemap& operator=(const Cubemap&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief Cubemap settings
         * @return
         */
        const TextureSettings& settings() const noexcept;

    private:
        TextureSettings settings_;

    };

}
