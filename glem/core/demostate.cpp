#include "demostate.hpp"

#include "inputmanager.hpp"

#include <glad/glad.h>

#include <util/log.hpp>

namespace  {
    const std::string TAG = "Demo";
}

namespace glem::core {

    void DemoState::onAttach() noexcept
    {
        const float positions[] {
             0.5f,  0.5f,
             0.5f, -0.5f,
            -0.5f, -0.5f,
            -0.5f,  0.5f,
        };

        const uint32_t indicies[] {
            0, 1, 3, 1, 2, 3
        };

        vao_.reset(new render::VertexArray{});
        program_.reset(new render::ShaderProgram{});

        const auto& vbo = std::make_shared<render::VertexBuffer>(render::InputLayout{{render::Float2, "positions"}}, positions, sizeof (positions));
        const auto& ibo = std::make_shared<render::IndexBuffer>(indicies, sizeof (indicies));

        vao_->append(vbo);
        vao_->append(ibo);

        const auto& vs = R"glsl(
                         #version 450 core

                         layout(location = 0) in vec4 positions;

                         uniform mat4 uViewProjection;

                         out vec2 uv;

                         void main() {
                             gl_Position = uViewProjection * vec4(positions.xy, 0.0f, 1.0f);
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450 core

                         layout(location = 0) out vec4 color;

                         void main() {
                             color = vec4(0.5f, 1.0f, 1.0f, 1.0f);
                         }
                         )glsl";

        program_->append(render::Shader::fromSource(vs, render::VS));
        program_->append(render::Shader::fromSource(ps, render::PS));

        if(!program_->link())
            util::Log::e(TAG, "Failed to link program.");

        camera_.reset(new render::Camera{glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f)});
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
        vao_->bind();

        program_->bind();
        program_->setUniform("uViewProjection", camera_->viewProjection());

        glDrawElements(GL_TRIANGLES,
                       static_cast<GLsizei>(vao_->indexBuffer()->count()),
                       GL_UNSIGNED_INT,
                       reinterpret_cast<const void*>(0));
    }

}
