#include "gamestate.hpp"
#include "level.hpp"

#include <util/log.hpp>

#include <filesystem>

namespace {
    const std::string TAG = "GameState";

    const std::string LEVELS_PATH  = "levels/";
    const std::string SHADERS_PATH = "shaders/";
    const std::string FONTS_PATH   = "fonts/";

    const std::string FONT = "desyrel.ttf";

    const uint32_t PADDING = 5.0f;

    const auto& width  = static_cast<uint32_t>(glem::core::Application::instance().window()->width());
    const auto& height = static_cast<uint32_t>(glem::core::Application::instance().window()->height());
}

namespace breakout {

    void GameState::onAttach() noexcept
    {
        camera_.reset(new glem::render::Camera{glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height))});

        if(!std::filesystem::exists(LEVELS_PATH)) {
            glem::util::Log::w(TAG, "Level directory doesn't exist.");
            glem::core::StateManager::pop();
        }

        for(const auto & entry : std::filesystem::directory_iterator(LEVELS_PATH)) {
            glem::util::Log::d(TAG, "Loading ", entry.path().string());

            breakout::Level level;

            if(level.load(entry.path().string(), width, height / 2))
                levels_.emplace_back(level);
            else
                glem::util::Log::w(TAG, "failed to load level ", entry.path().string());
        }

        if(!std::filesystem::exists(SHADERS_PATH)) {
            glem::util::Log::w(TAG, "Shaders directory doesn't exist.");
            glem::core::StateManager::pop();
        }

        levelShader_.reset(new glem::render::ShaderProgram{});
        levelShader_->append(glem::render::Shader::fromFile(SHADERS_PATH + "solid.vert.glsl", glem::render::VS));
        levelShader_->append(glem::render::Shader::fromFile(SHADERS_PATH + "solid.frag.glsl", glem::render::PS));

        if(!levelShader_->link()) {
            glem::util::Log::w(TAG, "Failed to link shader program.");
            glem::core::StateManager::pop();
        }

        std::vector<int> units;

        for(int i = 0; i < 32; ++i)
            units.push_back(i);

        levelShader_->bind();
        levelShader_->setUniform("u_view_projection", camera_->viewProjection());
        levelShader_->setUniform("u_textures", units.data(), static_cast<int>(units.size()));

        fontShader_.reset(new glem::render::ShaderProgram{});
        fontShader_->append(glem::render::Shader::fromFile(SHADERS_PATH + "font.vert.glsl", glem::render::VS));
        fontShader_->append(glem::render::Shader::fromFile(SHADERS_PATH + "font.frag.glsl", glem::render::PS));

        if(!fontShader_->link()) {
            glem::util::Log::w(TAG, "Failed to link shader program.");
            glem::core::StateManager::pop();
        }

        fontShader_->bind();
        fontShader_->setUniform("u_view_projection", camera_->viewProjection());
        fontShader_->setUniform("u_textures", units.data(), static_cast<int>(units.size()));

        const auto& size     = glm::vec2{200, 20};
        const auto& color    = glm::vec4{1.0f};
        const auto& position = glm::vec3{(width - size.x) / 2, PADDING, 0.0f};

        player_.reset(new glem::render::Drawable{position, size, color, levels_.back().textures()[3]});

        if(!std::filesystem::exists(FONTS_PATH)) {
            glem::util::Log::w(TAG, "Fonts directory doesn't exist.");
            glem::core::StateManager::pop();
        }

        font_.load(FONTS_PATH + FONT, 48);

        bg_.reset(new glem::render::Drawable{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec2{width, height}, color, levels_.back().textures()[0]});
    }

    void GameState::onDetach() noexcept
    {

    }

    void GameState::onEvent(glem::core::Event &event) noexcept
    {
        static_cast<void>(event);
    }

    void GameState::onUpdate(float dt) noexcept
    {
        if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::Escape))
            glem::core::Application::instance().window()->close();

        static const auto& velocity = 500.0f;

        auto position = player_->position();

        if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::Left)) {
            if(position.x >= 0.0f + PADDING * 2)
                position.x -= velocity * dt;
        }
        if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::Right)) {
            if(position.x <= width - player_->size().x - PADDING * 2)
                position.x += velocity * dt;
        }

        player_->setPosition(position);
    }

    void GameState::onDraw() noexcept
    {
        levelShader_->bind();

        glem::render::Renderer::begin();
        glem::render::Renderer::submit(bg_);

        for(const auto& l : levels_)
            l.draw();

        glem::render::Renderer::submit(player_);
        glem::render::Renderer::end();
        glem::render::Renderer::present();

        glem::core::Application::instance().window()->context().enableBlend();

        fontShader_->bind();

        const char* text = "Breakout";

        glem::render::Renderer::begin();
        glem::render::Renderer::submitText(text, glm::vec2{20.0f, 60.0f}, font_, glm::vec4{0.4f, 0.4f, 0.4f, 1.0f});
        glem::render::Renderer::end();
        glem::render::Renderer::present();

        glem::core::Application::instance().window()->context().enableBlend();
    }

}


