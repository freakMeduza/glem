#pragma once

#include "state.hpp"

#include <memory>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace glem::render {
    class Font;
    class Camera;
    class Texture;
    class Drawable;
    class ShaderProgram;
}

namespace glem::core {

    struct Point {
        float x {0.0f};
        float y {0.0f};
        float s {0.0f};
        float t {0.0f};
    };

    struct Atlas {
        uint32_t texture {0};

        uint32_t width  {0};
        uint32_t height {0};

        struct Glyph {
            uint8_t ascii;

            glm::vec2 size;
            glm::vec2 bearing;
            glm::vec2 advance;
            glm::vec2 offset;
        };

        std::vector<Glyph> glyphs;

        Atlas(FT_Face face, int size);
        ~Atlas();
    };

    class DebugState : public State {
    public:
        DebugState() = default;
        ~DebugState() = default;

        // State interface
        void onAttach() noexcept override;
        void onDetach() noexcept override;
        void onEvent(Event &event) noexcept override;
        void onUpdate(float dt) noexcept override;
        void onDraw() noexcept override;

    private:
        void init() noexcept;
        void draw() noexcept;
        void renderText(const std::string& text, Atlas* atlas, const glm::vec2& position, const glm::vec2& scale) noexcept;

        std::unique_ptr<Atlas>                 atlas_ {nullptr};
        std::shared_ptr<render::Camera>        camera_  {nullptr};
        std::shared_ptr<render::ShaderProgram> program_ {nullptr};

        std::vector<std::shared_ptr<render::Drawable>> sprites_;

        uint32_t vbo_ {0};
        uint32_t vao_ {0};

    };

}
