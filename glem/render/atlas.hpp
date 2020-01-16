#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "bindable.hpp"

namespace glem::render {

    class Surface;

    struct Glyph {
        uint8_t ascii {0};

        glm::vec2 size;
        glm::vec2 advance;
        glm::vec2 bearing;
        glm::vec2 uv;
    };

    class Atlas : public Bindable {
    public:
        Atlas(FT_Face face, uint32_t size);
        ~Atlas();

        // Bindable interface
        void bind(uint32_t unit = 0) noexcept override;

    private:
        std::vector<Glyph> glyphs_;

        std::shared_ptr<Surface> surface_ {nullptr};

    };

}
