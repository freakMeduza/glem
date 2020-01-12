#include "drawable.hpp"

namespace glem::render {

    Drawable::Drawable(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, const std::shared_ptr<Texture> &texture) :
        position_ {position},
        color_    {color},
        size_     {size},
        texture_  {texture}
    {
        uv_.emplace_back(0.0f, 0.0f);
        uv_.emplace_back(0.0f, 1.0f);
        uv_.emplace_back(1.0f, 1.0f);
        uv_.emplace_back(1.0f, 0.0f);
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

    void Drawable::setTexture(const std::shared_ptr<Texture> &value) noexcept
    {
        texture_ = value;
    }

    std::shared_ptr<Texture> Drawable::texture() const noexcept
    {
        return texture_;
    }

    const std::vector<glm::vec2> &Drawable::uv() const noexcept
    {
        return uv_;
    }

}
