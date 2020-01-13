#pragma once

#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace glem::render {

//    struct Glyph {
//        uint8_t character {0};

//        glm::ivec2 coord0  {glm::ivec2{0}};
//        glm::ivec2 coord1  {glm::ivec2{0}};
//        glm::ivec2 size    {glm::ivec2{0}};
//        glm::ivec2 bearing {glm::ivec2{0}};

//        uint32_t advance {0};
//    };

    class FontPrivate;

    class Font {
    public:
        Font();
        ~Font();

        bool makeAtlas(const std::string& font) noexcept;

    private:
        std::unique_ptr<FontPrivate> d_ptr_;

    };

}
