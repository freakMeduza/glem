#pragma once

#include "state.hpp"

#include <memory>
#include <vector>

namespace glem::render {
    class Font;
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
        std::shared_ptr<render::Camera>        camera_  {nullptr};
        std::shared_ptr<render::ShaderProgram> program_ {nullptr};

        std::vector<std::shared_ptr<render::Drawable>> sprites_;

    };

}
