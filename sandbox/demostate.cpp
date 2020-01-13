#include "demostate.hpp"

#include <render/drawable.hpp>
#include <render/renderer.hpp>

#include <util/log.hpp>

#include <time.h>

namespace  {
    const std::string TAG = "Demo";

    const auto& vs = R"glsl(
                     #version 450 core

                     layout(location = 0) in vec3  vPosition;
                     layout(location = 1) in vec4  vColor;
                     layout(location = 2) in vec2  vUv;
                     layout(location = 3) in float vSlot;

                     uniform mat4 uViewProjection;

                     out vec3  fPosition;
                     out vec4  fColor;
                     out vec2  fUv;
                     out float fSlot;

                     void main() {
                         gl_Position = uViewProjection * vec4(vPosition, 1.0f);

                         fPosition = vPosition;
                         fColor    = vColor;
                         fUv       = vUv;
                         fSlot     = vSlot;
                     }
                     )glsl";

    const auto& ps = R"glsl(
                     #version 450 core

                     layout(location = 0) out vec4 fragColor;

                     uniform vec2 uLightPosition;

                     uniform sampler2D uTexture[32];

                     in vec3  fPosition;
                     in vec4  fColor;
                     in vec2  fUv;
                     in float fSlot;

                     void main() {
                         float intensity = 1.0 / length(fPosition.xy - uLightPosition);

                         intensity = clamp(intensity, 0.0f, 1.0f);
                         intensity = sqrt(intensity);

                         intensity = intensity * 2.5f;

                         vec4 color = fColor;

                         if(fSlot >= 0.0f) {
                            color = texture2D(uTexture[int(fSlot)], fUv);
                         }

                         fragColor = color * intensity;
                     }
                     )glsl";

    std::vector<std::shared_ptr<glem::render::Texture>> textures;

    const std::string TEXTURE0 = "texture0.jpg";
    const std::string TEXTURE1 = "texture1.jpg";
    const std::string TEXTURE2 = "font_atlas.png";

    int used[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
}

void DemoState::onAttach() noexcept
{
    std::srand(time(nullptr));

    const auto& width  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& height = static_cast<float>(glem::core::Application::instance().window()->height());

    camera_.reset(new glem::render::Camera{glm::ortho(0.0f, width, 0.0f, height)});

    program_.reset(new glem::render::ShaderProgram{});

    program_->append(glem::render::Shader::fromSource(vs, glem::render::VS));
    program_->append(glem::render::Shader::fromSource(ps, glem::render::PS));

    if(!program_->link())
        glem::util::Log::e(TAG, "Failed to link program.");

    program_->bind();
    program_->setUniform("uViewProjection", camera_->viewProjection());
    program_->setUniform("uTexture", used, 10);

    textures.emplace_back(std::make_shared<glem::render::Texture>(TEXTURE0,
                                                                  TEXTURE0,
                                                                  glem::render::Wrap::ClampToBorder,
                                                                  glem::render::Filter::Linear,
                                                                  glem::render::Filter::Linear,
                                                                  0));

    textures.emplace_back(std::make_shared<glem::render::Texture>(TEXTURE1,
                                                                  TEXTURE1,
                                                                  glem::render::Wrap::ClampToBorder,
                                                                  glem::render::Filter::Linear,
                                                                  glem::render::Filter::Linear,
                                                                  1));

    glem::util::Log::d(TAG, "Number of textures: ", textures.size());

    for(float y = 0.0f; y < height; y += 40.0f) {
        static bool chose = true;
        chose = !chose;
        for(float x = 0.0f; x < width; x += 40.0f)
            sprites_.emplace_back(std::make_shared<glem::render::Drawable>(glm::vec3{x, y, 0.0f},
                                                                           glm::vec2{35.5f, 35.5f},
                                                                           glm::vec4{std::rand() % 1000 / 1000.0f, 0.0f, 1.0f, 1.0f},
                                                                           textures[chose ? 0 : 1]));
    }

    glem::util::Log::d(TAG, "Number of sprites: ", sprites_.size());
}

void DemoState::onDetach() noexcept
{
    textures.clear();
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
    static_cast<void>(dt);

    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::Escape))
        glem::core::Application::instance().window()->close();

//    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::W))
//        camera_->move(glem::render::Camera::Up,    dt);
//    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::S))
//        camera_->move(glem::render::Camera::Down,  dt);
//    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::D))
//        camera_->move(glem::render::Camera::Right, dt);
//    if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::A))
//        camera_->move(glem::render::Camera::Left,  dt);
}

void DemoState::onDraw() noexcept
{
    program_->bind();

    auto [x, y] = glem::core::InputManager::mouse().position();

    program_->setUniform("uLightPosition", glm::vec2{x, (static_cast<float>(glem::core::Application::instance().window()->height()) - y)});

    glem::render::Renderer::begin();

    for(auto s : sprites_)
        glem::render::Renderer::submit(s);

    glem::render::Renderer::end();
    glem::render::Renderer::present();
}
