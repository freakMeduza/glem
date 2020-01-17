#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

namespace glem::render {

    class Texture;

    /**
     * @brief The Glyph struct
     *
     * Describe character in atlas
     */
    struct Glyph {
        uint8_t ascii {0u};

        glm::vec2 size;
        glm::vec2 bearing;
        glm::vec2 advance;
        glm::vec2 uv;
    };

    /**
     * @brief The Font class
     *
     * Describe font atlas
     */
    class Font {
    public:
        Font() = default;
        ~Font() = default;

        /**
         * @brief load
         * @param font
         * @param size
         * @return
         */
        bool load(const std::string& font, uint32_t size) noexcept;

        /**
         * @brief atlas
         * @return
         */
        std::shared_ptr<Texture> atlas() const noexcept;

        /**
         * @brief glyphs
         * @return
         */
        const std::vector<Glyph>& glyphs() const noexcept;

    private:
        std::shared_ptr<Texture> atlas_ {nullptr};

        std::vector<Glyph> glyphs_;

    };

}
