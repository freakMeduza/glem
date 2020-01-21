#include "level.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

#include <util/log.hpp>

namespace {
    const std::string TAG = "Level";

    const std::string ASSETS_PATH  = "assets/";
}

namespace breakout {

    Level::Level()
    {

    }

    Level::~Level()
    {

    }

    bool Level::load(const std::string &filepath, uint32_t width, uint32_t height) noexcept
    {
        if(!std::filesystem::exists(ASSETS_PATH)) {
            glem::util::Log::w(TAG, "Assets directory doesn't exist.");
            glem::core::StateManager::pop();
        }

        for(const auto & entry : std::filesystem::directory_iterator(ASSETS_PATH)) {
            glem::util::Log::d(TAG, "Loading ", entry.path().string());

            glem::render::Properties p;
            p.wrap      = glem::render::Wrap::ClampToEdge;
            p.magFilter = glem::render::Filter::Linear;
            p.minFilter = glem::render::Filter::Linear;

            glem::render::Options o;
            o.verticalFlip = true;

            textures_.emplace_back(std::make_shared<glem::render::Texture>(entry.path().string(),
                                                                           entry.path().string(),
                                                                           p,
                                                                           o));
        }

        bricks_.clear();

        std::ifstream file{filepath, std::ios::in | std::ios::binary};

        if(file.is_open()) {
            std::string line;

            std::vector<std::vector<uint32_t>> tileData;

            while(std::getline(file, line)) {
                std::stringstream ss{line};

                uint32_t code {0};

                std::vector<uint32_t> row;

                while(ss >> code)
                    row.push_back(code);

                tileData.emplace_back(row);
            }

            file.close();

            if(!tileData.empty())
                init(tileData, width, height);
            else
                glem::util::Log::w(TAG, "No level found in file.");

            return true;
        }

        glem::util::Log::e(TAG, "Load level ", filepath, " failed.");

        return false;
    }

    void Level::draw() const noexcept
    {
        for(const auto& b : bricks_)
            glem::render::Renderer::submit(b);
    }

    bool Level::isReady() const noexcept
    {
        // TODO: drawables visible and don't destroyed.
        return true;
    }

    const std::vector<std::shared_ptr<glem::render::Texture> > &Level::textures() const noexcept
    {
        return textures_;
    }

    void Level::init(const std::vector<std::vector<uint32_t> > &tileData, uint32_t width, uint32_t height) noexcept
    {
        const auto& h = tileData.size();
        const auto& w = tileData.front().size();

        float unit_width  = width  / static_cast<float>(w);
        float unit_height = height / static_cast<float>(h);

        const auto& padding = 1.5f;

        for(uint32_t y = 0; y < h; ++y) {
            for(uint32_t x = 0 ; x < w; ++x) {
                // solid gray block, undestroyable
                const auto& index = tileData[y][x];

                glm::vec2 size     {unit_width - padding, unit_height - padding};
                glm::vec3 position {unit_width * x, (glem::core::Application::instance().window()->height() - unit_height) - unit_height * y, 0.0f};
                glm::vec4 color    {0.5f, 0.5f,0.5f, 1.0f};

                if(index == 1)
                    bricks_.emplace_back(std::make_shared<glem::render::Drawable>(position,
                                                                                  size,
                                                                                  color,
                                                                                  textures_[2]));
                else if(index > 1) {
                    switch (index) {
                    case 2: { color = glm::vec4{0.6f, 0.4f, 0.3f, 1.0f}; break; }
                    case 3: { color = glm::vec4{0.0f, 0.7f, 0.8f, 1.0f}; break; }
                    case 4: { color = glm::vec4{0.8f, 0.6f, 0.0f, 1.0f}; break; }
                    case 5: { color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}; break; }
                    }

                    bricks_.emplace_back(std::make_shared<glem::render::Drawable>(position,
                                                                                  size,
                                                                                  color,
                                                                                  textures_[1]));
                }
            }
        }
    }

}
