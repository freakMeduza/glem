#pragma once

#include "bindable.hpp"
#include "surface.hpp"

namespace glem::render {

    /**
     * @brief The Texture class
     *
     * Describing texture object
     */
    class Texture : public Bindable {
    public:
        Texture(const std::string& tag,
                const std::string& path,
                Wrap wrap,
                Filter minFilter,
                Filter magFilter,
                uint32_t slot = 0);

        Texture(const std::string& tag,
                uint32_t width,
                uint32_t height,
                Wrap wrap,
                Filter minFilter,
                Filter magFilter,
                Format format,
                uint32_t slot = 0);

        ~Texture() override;

        Texture(Texture&&) = delete;
        Texture(const Texture&) = delete;

        Texture& operator=(Texture&&) = delete;
        Texture& operator=(const Texture&&) = delete;

        // Bindable interface
        void bind() noexcept override;

        /**
         * @brief tag       Texture tag
         * @return
         */
        [[nodiscard]] std::string tag() const noexcept;

        /**
         * @brief width     Texture width
         * @return
         */
        uint32_t width() const noexcept;

        /**
         * @brief height    Texture height
         * @return
         */
        uint32_t height() const noexcept;

        /**
         * @brief setSurface
         * @param value
         * @return
         */
        bool setSurface(const Surface& value) noexcept;

    private:
        std::string tag_ {"Undefined"};

        uint32_t width_  {0};
        uint32_t height_ {0};

        uint32_t slot_ {0};

        Format format_;

    };

}
