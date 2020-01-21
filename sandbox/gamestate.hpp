#pragma once

#include <glem.hpp>

namespace breakout {

    class Level;

    class GameState : public glem::core::State {
    public:
        GameState() = default;
        ~GameState() override = default;

        // State interface
        void onAttach() noexcept override;
        void onDetach() noexcept override;
        void onEvent(glem::core::Event &event) noexcept override;
        void onUpdate(float dt) noexcept override;
        void onDraw() noexcept override;

    private:
        std::unique_ptr<glem::render::Camera>        camera_      {nullptr};
        std::unique_ptr<glem::render::ShaderProgram> levelShader_ {nullptr};
        std::unique_ptr<glem::render::ShaderProgram> fontShader_  {nullptr};

        glem::render::Font font_;

        std::vector<Level> levels_;

        std::shared_ptr<glem::render::Drawable> player_ {nullptr};
        std::shared_ptr<glem::render::Drawable> bg_     {nullptr};

    };

}
