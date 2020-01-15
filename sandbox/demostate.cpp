#include "demostate.hpp"

#include <render/drawable.hpp>
#include <render/renderer.hpp>

#include <fstream>

#include <util/log.hpp>

#include <time.h>

namespace {
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
                     uniform mat4 uViewProjection;

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

                         fragColor = color;// * intensity;
                     }
                     )glsl";

    std::vector<std::shared_ptr<glem::render::Texture>> textures;

    const std::string TEXTURE0 = "texture0.jpg";
    const std::string TEXTURE1 = "texture1.jpg";

    int used[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    const std::string ATLAS = "atlas.png";

    bool exists(const std::string& path) noexcept {
        std::ifstream file{path};
        return file.good();
    }

    const uint32_t ATLAS_DIMENSION = 1024;
    const uint32_t ATLAS_SLOT      = 512;

    bool make(const std::string& path) noexcept {
        glem::util::Log::d(TAG, __FUNCTION__);

        std::vector<uint8_t> buffer;
        buffer.resize(ATLAS_DIMENSION * ATLAS_DIMENSION * 4);

        uint32_t index = 0;

        for(uint32_t y = 0; y < ATLAS_DIMENSION; ++y) {
            for(uint32_t x = 0; x < ATLAS_DIMENSION; ++x) {
                int row    = y / ATLAS_SLOT;
                int column = x / ATLAS_SLOT;

                /*
                    columns
                    ---------------------
                   |          |          |
                   |    1     |     2    |
                   |          |          |
             rows  |          |          |
                   | -----------------   |
                   |          |          |
                   |    3     |     4    |
                   |          |          |
                   |          |          |
                    ---------------------
                */

                if(row == 0 && column == 0) {
                    // 1st rect red
                    buffer[index++] = 255;
                    buffer[index++] = 0;
                    buffer[index++] = 0;
                    buffer[index++] = 255;
                }
                else if(row == 0 && column == 1) {
                    // 2rd rect blue
                    buffer[index++] = 0;
                    buffer[index++] = 0;
                    buffer[index++] = 255;
                    buffer[index++] = 255;
                }
                else if(row == 1 && column == 0) {
                    // 3nd rect green
                    buffer[index++] = 0;
                    buffer[index++] = 255;
                    buffer[index++] = 0;
                    buffer[index++] = 255;
                }
                else {
                    // 4th rect white
                    buffer[index++] = 255;
                    buffer[index++] = 255;
                    buffer[index++] = 255;
                    buffer[index++] = 255;
                }
            }
        }

        glem::render::Surface s{buffer, ATLAS_DIMENSION, ATLAS_DIMENSION, glem::render::Format::RGBA};

        return s.save(path);
    }
}

void DemoState::onAttach() noexcept
{
    const auto& width  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& height = static_cast<float>(glem::core::Application::instance().window()->height());

    camera_.reset(new glem::render::Camera{glm::ortho(0.0f, width, 0.0f, height)});
    camera_->setSensitivity(100.0f);

    program_.reset(new glem::render::ShaderProgram{});

    program_->append(glem::render::Shader::fromSource(vs, glem::render::VS));
    program_->append(glem::render::Shader::fromSource(ps, glem::render::PS));

    if(!program_->link())
        glem::util::Log::e(TAG, "Failed to link program.");

    /* ******************************************************************************************************* */

    program_->bind();
    program_->setUniform("uViewProjection", camera_->viewProjection());
    program_->setUniform("uTexture", used, 10);

    const auto& side = 200.0f;

    auto&& position = glm::vec3{(width - side) / 4, (height - side) / 4, 0.0f};
    auto&& size     = glm::vec2{side, side};
    auto&& color    = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};

    sprites_.emplace_back(std::make_shared<glem::render::Drawable>(position, size, color));

    font_.reset(new glem::render::Font{});
    font_->load("OCRAEXT.TTF");
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

//    auto [x, y] = glem::core::InputManager::mouse().position();

//    program_->setUniform("uLightPosition", glm::vec2{x, (static_cast<float>(glem::core::Application::instance().window()->height()) - y)});

    glem::render::Renderer::begin();

//    for(auto s : sprites_)
//        glem::render::Renderer::submit(s);
    glem::render::Renderer::submitText("!", glm::vec2{100.0f, (glem::core::Application::instance().window()->height() - 96.0f) / 2.0f}, font_);


    glem::render::Renderer::end();
    glem::render::Renderer::present();

    glem::core::StateManager::pop();
}
