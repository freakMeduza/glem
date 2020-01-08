#include "demostate.hpp"

#include <render/drawable.hpp>
#include <render/renderer.hpp>

#include <util/log.hpp>

#include <time.h>

namespace  {
const std::string TAG = "Demo";
}

void DemoState::onAttach() noexcept
{
    glem::render::Renderer::init();

    std::srand(time(nullptr));

    const auto& width  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& height = static_cast<float>(glem::core::Application::instance().window()->height());

    glem::util::Log::d(TAG, "Width: ", width, " Height: ", height);

    for(float y = 0.0f; y < height; y += 3.0f) {
        for(float x = 0.0f; x < width; x += 3.0f) {
            sprites_.emplace_back(std::make_shared<glem::render::Drawable>(glm::vec3{x, y, 0.0f},
                                                                     glm::vec4{std::rand() % 1000 / 1000.0f, 0.0f, 1.0f, 1.0f},
                                                                     glm::vec2{2.5f, 2.5f}));
        }
    }

    glem::util::Log::d(TAG, "Number of sprites: ", sprites_.size());

    camera_.reset(new glem::render::Camera{glm::ortho(0.0f, width, 0.0f, height)});

    program_.reset(new glem::render::ShaderProgram{});

    const auto& vs = R"glsl(
                     #version 450 core

                     layout(location = 0) in vec3 vPosition;
                     layout(location = 1) in vec4 vColor;

                     uniform mat4 uViewProjection;

                     out vec3 fPosition;
                     out vec4 fColor;

                     void main() {
                     gl_Position = uViewProjection * vec4(vPosition, 1.0f);

                     fPosition = vPosition;
                     fColor    = vColor;
                     }
                     )glsl";

    const auto& ps = R"glsl(
                     #version 450 core

                     layout(location = 0) out vec4 color;

                     uniform vec2 uLightPosition;

                     in vec3 fPosition;
                     in vec4 fColor;

                     void main() {
                     float intensity = 1.0 / length(fPosition.xy - uLightPosition);

                     intensity = clamp(intensity, 0.0f, 1.0f);
                     intensity = sqrt(intensity);

                     intensity = intensity * 2.5f;

                     color = fColor * intensity;
                     }
                     )glsl";

    program_->append(glem::render::Shader::fromSource(vs, glem::render::VS));
    program_->append(glem::render::Shader::fromSource(ps, glem::render::PS));

    if(!program_->link())
        glem::util::Log::e(TAG, "Failed to link program.");
}

void DemoState::onDetach() noexcept
{

}

void DemoState::onEvent(glem::core::Event &event) noexcept
{
    if(event.type() == glem::core::Event::MouseScrolled) {
        auto e = static_cast<glem::core::MouseEvent*>(&event);

        glem::util::Log::d(TAG, "MouseScrollEvent ----> offset ", e->offset().second);
    }
}

void DemoState::onUpdate(float dt) noexcept
{
    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::Escape))
        glem::core::Application::instance().window()->close();

    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::W))
        camera_->move(glem::render::Camera::Up, dt);
    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::S))
        camera_->move(glem::render::Camera::Down, dt);
    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::D))
        camera_->move(glem::render::Camera::Right, dt);
    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::A))
        camera_->move(glem::render::Camera::Left, dt);
}

void DemoState::onDraw() noexcept
{
    program_->bind();
    program_->setUniform("uViewProjection", camera_->viewProjection());

    auto [x, y] = glem::core::InputManager::mouse().position();

            program_->setUniform("uLightPosition", glm::vec2{x, (static_cast<float>(glem::core::Application::instance().window()->height()) - y)});

    glem::render::Renderer::begin();

    for(size_t i = 0; i < sprites_.size(); ++i) {
        glem::render::Renderer::submit(sprites_[i]);
    }

    glem::render::Renderer::end();

    glem::render::Renderer::release();
}
