#include "drawable.hpp"

namespace glem::render {

    Drawable::Drawable(const glm::vec3 &position, const glm::vec4 &color, const glm::vec2 &size) :
        position_ {position},
        color_    {color},
        size_     {size}
    {

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

}
