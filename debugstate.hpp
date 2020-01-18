#pragma once

#include "state.hpp"

#include <memory>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <render/texture.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <render/font.hpp>

namespace glem::render {
    class Camera;
    class Texture;
    class Drawable;
    class ShaderProgram;
}

namespace glem::core {

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

        std::shared_ptr<render::Camera>        camera_    {nullptr};
        std::shared_ptr<render::ShaderProgram> program_   {nullptr};

        render::Font atlas_;

        std::vector<std::shared_ptr<render::Drawable>> sprites_;

        uint32_t vbo_ {0};
        uint32_t vao_ {0};

    };

}
