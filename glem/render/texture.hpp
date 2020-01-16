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
                const Properties& properties = {},
                const Options& options = {});

        Texture(const std::string& tag,
                uint32_t width,
                uint32_t height,
                const Properties& properties = {});

        virtual ~Texture() override;

        Texture(Texture&&) = delete;
        Texture(const Texture&) = delete;

        Texture& operator=(Texture&&) = delete;
        Texture& operator=(const Texture&&) = delete;

        // Bindable interface
        void bind(uint32_t unit = 0) noexcept override;

        /**
         * @brief tag       Texture tag
         * @return
         */
        [[nodiscard]] std::string tag() const noexcept;

        /**
         * @brief width     Texture width
         * @return
         */
        [[nodiscard]] uint32_t width() const noexcept;

        /**
         * @brief height    Texture height
         * @return
         */
        [[nodiscard]] uint32_t height() const noexcept;

        /**
         * @brief setSurface
         * @param value
         * @return
         */
        bool setSurface(const std::shared_ptr<Surface>& value) noexcept;

        /**
         * @brief surface
         * @return
         */
        [[nodiscard]] std::shared_ptr<Surface> surface() const noexcept;

    private:
        std::string tag_ {"Undefined"};

        uint32_t width_  {0};
        uint32_t height_ {0};

        Format format_;

        std::shared_ptr<Surface> surface_ {nullptr};

    };

}
