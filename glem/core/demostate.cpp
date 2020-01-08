#include "demostate.hpp"

#include "inputmanager.hpp"

#include <render/drawable.hpp>
#include <render/renderer.hpp>

#include <util/log.hpp>

#include <time.h>

namespace  {
    const std::string TAG = "Demo";
}

namespace glem::core {

    void DemoState::onAttach() noexcept
    {
        render::Renderer::init();

        std::srand(time(nullptr));

        const auto& width  = static_cast<float>(Application::instance().window()->width());
        const auto& height = static_cast<float>(Application::instance().window()->height());

        util::Log::d(TAG, "Width: ", width, " Height: ", height);

        for(float y = 0.0f; y < height; y += 5.0f) {
            for(float x = 0.0f; x < width; x += 5.0f) {
                sprites_.emplace_back(std::make_shared<render::Drawable>(glm::vec3{x, y, 0.0f},
                                                                         glm::vec4{std::rand() % 1000 / 1000.0f, 0.0f, 1.0f, 1.0f},
                                                                         glm::vec2{4.0f, 4.0f}));
            }
        }

        util::Log::d(TAG, "Number of sprites: ", sprites_.size());

        camera_.reset(new render::Camera{glm::ortho(0.0f, width, 0.0f, height)});

        program_.reset(new render::ShaderProgram{});

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

        program_->append(render::Shader::fromSource(vs, render::VS));
        program_->append(render::Shader::fromSource(ps, render::PS));

        if(!program_->link())
            util::Log::e(TAG, "Failed to link program.");
    }

    void DemoState::onDetach() noexcept
    {

    }

    void DemoState::onEvent(Event &event) noexcept
    {
        if(event.type() == Event::MouseScrolled) {
            auto e = static_cast<MouseEvent*>(&event);

            util::Log::d(TAG, "MouseScrollEvent ----> offset ", e->offset().second);
        }
    }

    void DemoState::onUpdate(float dt) noexcept
    {
        if(InputManager::keyboard().isKeyPressed(Keyboard::Escape))
            Application::instance().window()->close();

        if(InputManager::keyboard().isKeyPressed(Keyboard::W))
            camera_->move(render::Camera::Up, dt);
        if(InputManager::keyboard().isKeyPressed(Keyboard::S))
            camera_->move(render::Camera::Down, dt);
        if(InputManager::keyboard().isKeyPressed(Keyboard::D))
            camera_->move(render::Camera::Right, dt);
        if(InputManager::keyboard().isKeyPressed(Keyboard::A))
            camera_->move(render::Camera::Left, dt);
    }

    void DemoState::onDraw() noexcept
    {
        program_->bind();
        program_->setUniform("uViewProjection", camera_->viewProjection());

        auto [x, y] = InputManager::mouse().position();

        program_->setUniform("uLightPosition", glm::vec2{x, (static_cast<float>(Application::instance().window()->height()) - y)});

        render::Renderer::begin();

        for(size_t i = 0; i < sprites_.size(); ++i) {
            render::Renderer::submit(sprites_[i]);
        }

        render::Renderer::end();

        render::Renderer::release();
    }

}
