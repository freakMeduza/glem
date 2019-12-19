#pragma once

#include "properties.hpp"

#include <string>
#include <vector>
#include <optional>

namespace glem::render {

    /**
     * @brief The Surface class
     *
     * Describing image surface
     */
    class Surface {
    public:
        Surface(const std::vector<uint8_t>& pixels,
                uint32_t width,
                uint32_t height,
                Format format = Format{});

        ~Surface() = default;

        Surface(Surface&&) = default;
        Surface& operator=(Surface&&) = default;

        Surface(const Surface&) = delete;
        Surface& operator=(const Surface&) = delete;

        /**
         * @brief load      Load image
         * @param path      filepath
         * @return
         */
        static std::optional<Surface> load(const std::string& path) noexcept;

        /**
         * @brief save      Save image
         * @param path      filepath (suffix will be replace depend format)
         * @return
         */
        bool save(const std::string& path) const noexcept;

        /**
         * @brief width     Image width
         * @return
         */
        uint32_t width() const noexcept;

        /**
         * @brief height    Image height
         * @return
         */
        uint32_t height() const noexcept;

        /**
         * @brief format    Image format
         * @return
         */
        Format format() const noexcept;

        /**
         * @brief pixels    Image data
         * @return
         */
        const std::vector<uint8_t>& pixels() const noexcept;

    private:
        std::vector<uint8_t> pixels_;

        uint32_t width_  {0};
        uint32_t height_ {0};

        Format format_ {Format::Unspecified};

    };

}
