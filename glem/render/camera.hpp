#pragma once

#include "moveable.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace glem::render {

    class Camera : public Moveable {
    public:
        ~Camera() = default;

        Camera(Camera&&) = delete;
        Camera(const Camera&) = delete;

        Camera& operator=(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;

        Camera(const glm::mat4& projection);
        Camera(float left, float right, float bottom, float top);

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
         * @brief setSensitivity
         * @param value
         */
        void setSensitivity(float value) noexcept;

        /**
         * @brief sensitivity
         * @return
         */
        [[nodiscard]] float sensitivity() const noexcept;

        /**
         * @brief view
         * @return
         */
        [[nodiscard]] const glm::mat4& view() const noexcept;

        /**
         * @brief projection
         * @return
         */
        [[nodiscard]] const glm::mat4& projection() const noexcept;

        /**
         * @brief viewProjection
         * @return
         */
        [[nodiscard]] const glm::mat4& viewProjection() const noexcept;

        // Moveable interface
        void move(Movement movement, float dt) noexcept override;

    private:
        void update() noexcept;

        glm::mat4 view_           {glm::mat4(1.0f)};
        glm::mat4 projection_     {glm::mat4(1.0f)};
        glm::mat4 viewProjection_ {glm::mat4(1.0f)};

        glm::vec3 position_ {glm::vec3(0.0f)};

        float sensitivity_ {0.5f};
        float rotation_    {0.0f};

    };

}
