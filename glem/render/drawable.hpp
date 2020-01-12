#pragma once

#include <glm/glm.hpp>

namespace glem::render {

    class Drawable {
    public:
        Drawable() = default;
        ~Drawable() = default;

        Drawable(const glm::vec3& position,
                 const glm::vec2& size,
                 const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});

        Drawable(Drawable&&) = default;
        Drawable(const Drawable&) = default;

        Drawable& operator=(Drawable&&) = default;
        Drawable& operator=(const Drawable&) = default;

        /**
         * @brief setPosition
         * @param value
         */
        void setPosition(const glm::vec3& value) noexcept;

        /**
         * @brief position
         * @return
         */
        [[nodiscard]] const glm::vec3& position() const noexcept;

        /**
         * @brief setColor
         * @param value
         */
        void setColor(const glm::vec4& value) noexcept;

        /**
         * @brief color
         * @return
         */
        [[nodiscard]] const glm::vec4& color() const noexcept;

        /**
         * @brief setSize
         * @param value
         */
        void setSize(const glm::vec2& value) noexcept;

        /**
         * @brief size
         * @return
         */
        [[nodiscard]] const glm::vec2& size() const noexcept;

    private:
        glm::vec3 position_;
        glm::vec4 color_;
        glm::vec2 size_;

    };

}
