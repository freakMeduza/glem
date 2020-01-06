#pragma once

#include "state.hpp"

#include <glem.hpp>

#include <memory>

namespace glem::core {

    class DemoState : public State {
    public:
        DemoState() = default;
        ~DemoState() override = default;

        // State interface
        void onAttach() noexcept override;
        void onDetach() noexcept override;
        void onEvent(Event &event) noexcept override;
        void onUpdate(float dt) noexcept override;
        void onDraw() noexcept override;

    private:
        std::unique_ptr<render::Camera> camera_ {nullptr};

        std::unique_ptr<render::ShaderProgram> program_ {nullptr};
        std::unique_ptr<render::VertexArray>   vao_     {nullptr};

    };

}
