#include "drawable.hpp"

namespace glem::render {

    Drawable::Drawable(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, const std::shared_ptr<Texture> &texture, uint32_t atlasIndex) :
        position_   {position},
        color_      {color},
        size_       {size},
        texture_    {texture},
        atlasIndex_ {atlasIndex}
    {
        update();
    }

    void Drawable::setPosition(const glm::vec3 &value) noexcept
    {
        position_ = value;
    }

    const glm::vec3 &Drawable::position() const noexcept
    {
        return position_;
    }

    void Drawable::setColor(const glm::vec4 &value) noexcept
    {
        color_ = value;
    }

    const glm::vec4 &Drawable::color() const noexcept
    {
        return color_;
    }

    void Drawable::setSize(const glm::vec2 &value) noexcept
    {
        size_ = value;
    }

    const glm::vec2 &Drawable::size() const noexcept
    {
        return size_;
    }

    void Drawable::setUV(const std::vector<glm::vec2> &value) noexcept
    {
        uv_ = value;
    }

    const std::vector<glm::vec2> &Drawable::uv() const noexcept
    {
        return uv_;
    }

    void Drawable::setTexture(const std::shared_ptr<Texture> &value) noexcept
    {
        texture_ = value;
    }

    std::shared_ptr<Texture> Drawable::texture() const noexcept
    {
        return texture_;
    }

    void Drawable::setAtlasIndex(uint32_t value) noexcept
    {
        atlasIndex_ = value;

        update();
    }

    uint32_t Drawable::atlasIndex() const noexcept
    {
        return atlasIndex_;
    }

    void Drawable::update() noexcept
    {
        uv_.clear();

        uv_.emplace_back(0.0f, 0.0f);
        uv_.emplace_back(0.0f, 1.0f);
        uv_.emplace_back(1.0f, 1.0f);
        uv_.emplace_back(1.0f, 0.0f);

        if(texture_ && texture_->isAtlas()) {
            auto offset = [this]() {
                int column = atlasIndex_ % texture_->columns();
                int row    = atlasIndex_ / texture_->columns();

                float xoffset = static_cast<float>(column) / static_cast<float>(texture_->columns());
                float yoffset = static_cast<float>(row)    / static_cast<float>(texture_->columns());

                return glm::vec2{xoffset, yoffset};
            };

            const auto& columns = texture_->columns();

            for(size_t i = 0; i < 4; ++i) {
                const auto& x = uv_[i].x;
                const auto& y = uv_[i].y;

                uv_[i] = glm::vec2{x / columns + offset().x, y / columns + offset().y};
            }
        }
    }

}
