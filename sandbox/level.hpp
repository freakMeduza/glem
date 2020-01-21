#pragma once

#include <glem.hpp>

#include <string>
#include <vector>

namespace breakout {

    class Level {
    public:
        Level();
        ~Level();

        bool load(const std::string& filepath, uint32_t width, uint32_t height) noexcept;
        void draw() const noexcept;
        bool isReady() const noexcept;

        const std::vector<std::shared_ptr<glem::render::Texture>>& textures() const noexcept;

    private:
        void init(const std::vector<std::vector<uint32_t>>& tileData, uint32_t width, uint32_t height) noexcept;

        std::vector<std::shared_ptr<glem::render::Drawable>> bricks_;
        std::vector<std::shared_ptr<glem::render::Texture>>  textures_;

    };

}
