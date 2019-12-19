#pragma once

#include <glm/glm.hpp>

namespace glem::render {

    class Camera {
    public:
        Camera() = default;
        ~Camera() = default;

        Camera(Camera&&) = delete;
        Camera(const Camera&) = delete;

        Camera& operator=(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;

        Camera(const glm::mat4& projection);
        Camera(float left, float right, float bottom, float top);

        void setProjection(const glm::mat4& value) noexcept;
        void setProjection(float left, float right, float bottom, float top) noexcept;

    private:
        glm::mat4 view_       {glm::mat4(1.0f)};
        glm::mat4 projection_ {glm::mat4(1.0f)};

        glm::vec3 position {glm::vec3{1.0f}};

        float rotation_ {0.0f};
    };

}
