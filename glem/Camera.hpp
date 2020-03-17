#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glem {

    enum Movement {
        Forward,
        Backward,
        Left,
        Right
    };

    class Camera {
    public:
        Camera();
        ~Camera();

        Camera(const glm::vec3& pos,
               const glm::vec3& wu = glm::vec3{0.0f, 1.0f, 0.0f},
               float y = -90.0f,
               float p = 0.0f);

        glm::mat4 viewMatrix() const noexcept;

        void processMouse(float x_offset, float y_offset) noexcept;
        void processKeyboard(Movement movement, float dt) noexcept;

        glm::vec3 position {glm::vec3{0.0f}};
        glm::vec3 front    {glm::vec3{0.0f}};
        glm::vec3 right    {glm::vec3{0.0f}};
        glm::vec3 up       {glm::vec3{0.0f}};

        glm::vec3 worldUp  {glm::vec3{0.0f}};

        float yaw   {0.0f};
        float pitch {0.0f};

        float speed       {3.0f};
        float sensitivity {0.05f};

    private:
        void update() noexcept;
    };

}
