#include "debugstate.hpp"

#include "window.hpp"
#include "application.hpp"
#include "inputmanager.hpp"
#include "statemanager.hpp"

#include <render/camera.hpp>
#include <render/renderer.hpp>
#include <render/drawable.hpp>
#include <render/shaderprogram.hpp>
#include <render/vertexbuffer.hpp>
#include <render/vertexarray.hpp>

#include <render/font.hpp>

#include <glad/glad.h>

#include <util/log.hpp>
#include <util/timer.hpp>

#include <glm/glm.hpp>

#include "stb_image_write.h"

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

                     uniform vec2 unif_light_position  = vec2(100.0f, 100.0f);

                     const vec4  const_ambient         = vec4(0.3f, 0.3f, 0.3f, 1.0f);
                     const vec4  const_light_color     = vec4(0.999f, 0.999f, 0.999f, 1.0f);
                     const float const_light_intensity = 150.0f;

                     uniform sampler2D unif_textures[32];

                     in vec3  frag_position;
                     in vec4  frag_color;
                     in vec2  frag_uv;
                     in float frag_unit;

                     void main() {
                         vec4 color = frag_color;

                         if(frag_unit >= 0.0f)
                            color = texture2D(unif_textures[int(frag_unit)], frag_uv);

                         if(color.a < 1.0f)
                            discard;

                         float distance = distance(unif_light_position, frag_position.xy);
                         float diffuse  = 0.0f;

                         if(distance <= const_light_intensity)
                             diffuse = 1.0f - abs(distance / const_light_intensity);

                         out_color = vec4(min(color.rgb * ((const_light_color.rgb * diffuse)+ const_ambient.rgb), color.rgb), 1.0f);
                     }
                     )glsl";

//    const auto& vs_text = R"glsl(
//                          #version 450 core

//                          layout(location = 0) in vec3  vert_position;
//                          layout(location = 1) in vec4  vert_color;
//                          layout(location = 2) in vec2  vert_uv;
//                          layout(location = 3) in float vert_unit;

//                          out vec3  frag_position;
//                          out vec4  frag_color;
//                          out vec2  frag_uv;
//                          out float frag_unit;

//                          uniform mat4 unif_view_projection;

//                          void main() {
//                              gl_Position = unif_view_projection * vec4(vert_position, 1.0f);

//                              frag_position = vert_position;
//                              frag_color    = vert_color;
//                              frag_uv       = vert_uv;
//                              frag_unit     = vert_unit;
//                          }
//                          )glsl";

//    const auto& ps_text = R"glsl(
//                          #version 450 core

//                          layout(location = 0) out vec4 out_color;

//                          uniform sampler2D unif_textures[32];

//                          in vec3  frag_position;
//                          in vec4  frag_color;
//                          in vec2  frag_uv;
//                          in float frag_unit;

//                          void main() {
//                              out_color = vec4(1.0f, 1.0f, 1.0f, texture2D(unif_textures[int(frag_unit)], frag_uv).r) * frag_color;
//                          }
//                          )glsl";

    const auto& WIDTH  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& HEIGHT = static_cast<float>(glem::core::Application::instance().window()->height());

    const char* FONT     = "OCRAEXT.TTF";
    const char* TEXTURE0 = "texture0.jpg";
    const char* TEXTURE1 = "texture1.jpg";
}

namespace glem::core {

    void DebugState::onAttach() noexcept
    {
        init();
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

        auto pos = InputManager::mouse().position();
        pos.y = HEIGHT - pos.y;

        program_->setUniform("unif_light_position", pos);
    }

    void DebugState::onDraw() noexcept
    {
//        render::Renderer::begin();
//        render::Renderer::submitText("Hello! What's up?", {200.0f, 100.0f}, atlas_, {0.5f, 0.7f, 0.8f, 1.0f}, 2.0f);
//        render::Renderer::end();
//        render::Renderer::present();

        draw();
    }

    void DebugState::init() noexcept
    {
        camera_.reset(new render::Camera{glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT)});

        program_.reset(new render::ShaderProgram{});
        program_->append(render::Shader::fromSource(vs, render::VS));
        program_->append(render::Shader::fromSource(ps, render::PS));

        if(!program_->link()) {
            util::Log::e(TAG, "Failed to link shader program.");
            StateManager::pop();
        }

        std::vector<int> used;
        for(int i = 0; i < 33; ++i)
            used.push_back(i);

        program_->bind();
        program_->setUniform("unif_view_projection", camera_->viewProjection());
        program_->setUniform("unif_textures", used.data(), used.size());

        atlas_.load(FONT, 48);

        const auto& side = 50.0f;

        render::Properties p;
        p.wrap      = render::Wrap::ClampToEdge;
        p.magFilter = render::Filter::Linear;
        p.minFilter = render::Filter::Linear;

        render::Options o;
        o.verticalFlip = true;

        std::vector<std::shared_ptr<render::Texture>> textures;
        textures.emplace_back(std::make_shared<render::Texture>(TEXTURE0, TEXTURE0, p, o));
        textures.emplace_back(std::make_shared<render::Texture>(TEXTURE1, TEXTURE1, p, o));

//        sprites_.emplace_back(std::make_shared<render::Drawable>(glm::vec3{100.0f, 100.0f, 0.0f}, glm::vec2{300.0f, 300.0f}, glm::vec4{1.0f}, textures[0]));
//        sprites_.emplace_back(std::make_shared<render::Drawable>(glm::vec3{800.0f, 100.0f, 0.0f}, glm::vec2{300.0f, 300.0f}, glm::vec4{1.0f}, textures[1]));

        for(float y = 0.0f; y < HEIGHT; y += side) {
            for(float x = 0.0f; x < WIDTH; x+= side) {
                sprites_.emplace_back(std::make_shared<render::Drawable>(glm::vec3{x, y, 0.0f},
                                                                         glm::vec2{side - 1.0f, side - 1.0f},
                                                                         glm::vec4{1.0f},
                                                                         textures[std::rand() % 2]));
            }
        }

        util::Log::d(TAG, "Number of sprites: ", sprites_.size());
    }

    void DebugState::draw() noexcept
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
