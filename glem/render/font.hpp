#pragma once

#include <glm/glm.hpp>

#include <map>
#include <vector>
#include <memory>

namespace glem::render {

    class Texture;

    struct Glyph {
        std::vector<glm::vec2> uv;

        glm::vec2 size;
        glm::vec2 offset;

        float advance;
    };

    class Font {
    public:
        Font() = default;
        ~Font() = default;

        bool load(const std::string& font) noexcept;

        std::shared_ptr<Texture> atlas() const noexcept;
        std::map<uint8_t, Glyph> font() const noexcept;

    private:
        std::map<uint8_t, Glyph> font_;

        std::shared_ptr<Texture> atlas_ {nullptr};

    };

}
