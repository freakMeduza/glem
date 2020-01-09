#pragma once

#include <glem.hpp>

#include <memory>

class DemoState : public glem::core::State {
public:
    DemoState() = default;
    ~DemoState() override = default;

    // State interface
    void onAttach() noexcept override;
    void onDetach() noexcept override;
    void onEvent(glem::core::Event &event) noexcept override;
    void onUpdate(float dt) noexcept override;
    void onDraw() noexcept override;

private:
    std::unique_ptr<glem::render::Camera> camera_ {nullptr};

    std::unique_ptr<glem::render::ShaderProgram> program_ {nullptr};

    std::vector<std::shared_ptr<glem::render::Drawable>> sprites_;

};
