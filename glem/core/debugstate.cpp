#include "debugstate.hpp"

#include "window.hpp"
#include "application.hpp"
#include "inputmanager.hpp"
#include "statemanager.hpp"

#include <render/camera.hpp>
#include <render/texture.hpp>
#include <render/renderer.hpp>
#include <render/drawable.hpp>
#include <render/shaderprogram.hpp>

#include <util/log.hpp>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <fstream>
#include <map>

namespace {
    const std::string TAG = "Debug";

    const auto& vs = R"glsl(
                     #version 450 core

                     layout(location = 0) in vec3  vert_position;
                     layout(location = 1) in vec4  vert_color;
                     layout(location = 2) in vec2  vert_uv;
                     layout(location = 3) in float vert_unit;

                     uniform mat4 unif_view_projection;

                     out vec3  frag_position;
                     out vec4  frag_color;
                     out vec2  frag_uv;
                     out float frag_unit;

                     void main() {
                         gl_Position = unif_view_projection * vec4(vert_position, 1.0f);

                         frag_position = vert_position;
                         frag_color    = vert_color;
                         frag_uv       = vert_uv;
                         frag_unit     = vert_unit;
                     }
                     )glsl";

    const auto& ps = R"glsl(
                     #version 450 core

                     layout(location = 0) out vec4 out_color;

                     uniform vec2 unif_light_position;

                     uniform sampler2D unif_textures[32];

                     in vec3  frag_position;
                     in vec4  frag_color;
                     in vec2  frag_uv;
                     in float frag_unit;

                     void main() {
                         float intensity = 1.0 / length(frag_position.xy - unif_light_position);

                         intensity = clamp(intensity, 0.0f, 1.0f);
                         intensity = sqrt(intensity);

                         intensity = intensity * 2.5f;

                         vec4 color = frag_color;

                         if(frag_unit >= 0.0f) {
                            color = texture2D(unif_textures[int(frag_unit)], frag_uv);
                         }

                         out_color = color * intensity;
                     }
                     )glsl";

    const auto& WIDTH  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& HEIGHT = static_cast<float>(glem::core::Application::instance().window()->height());
}

namespace glem::core {

    void DebugState::onAttach() noexcept
    {
        camera_.reset(new render::Camera{glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT)});

        program_.reset(new render::ShaderProgram{});
        program_->append(render::Shader::fromSource(vs, render::VS));
        program_->append(render::Shader::fromSource(ps, render::PS));

        if(!program_->link()) {
            util::Log::e(TAG, "Failed to link shader program.");
            StateManager::pop();
        }

        program_->bind();
        program_->setUniform("unif_view_projection", camera_->viewProjection());

        const auto& side = 500.0f;
        const auto& x = (WIDTH  - side) / 2;
        const auto& y = (HEIGHT - side) / 2;

        sprites_.emplace_back(std::make_shared<render::Drawable>(glm::vec3{x, y, 0.0f}, glm::vec2{side, side}, glm::vec4{0.5f, 0.7f, 0.8f, 1.0f}));
    }

    void DebugState::onDetach() noexcept
    {

    }

    void DebugState::onEvent(Event &event) noexcept
    {
        static_cast<void>(event);
    }

    void DebugState::onUpdate(float dt) noexcept
    {
        static_cast<void>(dt);

        if(InputManager::keyboard().isKeyPressed(Keyboard::Escape))
            Application::instance().window()->close();

        auto position = InputManager::mouse().position();
        position.y = HEIGHT - position.y;

        program_->setUniform("unif_light_position", position);
    }

    void DebugState::onDraw() noexcept
    {
        if(sprites_.empty()) {
            util::Log::w(TAG, "No available sprites for rendering.");
            return;
        }

        render::Renderer::begin();

        for(const auto& s : sprites_)
            render::Renderer::submit(s);

        render::Renderer::end();
        render::Renderer::present();
    }

}
