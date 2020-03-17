#include "Camera.hpp"

namespace glem {

    Camera::Camera()
    {

    }

    Camera::~Camera()
    {

    }

    Camera::Camera(const glm::vec3 &pos, const glm::vec3 &wu, float y, float p) :
        position {pos},
        front    {glm::vec3{0.0f, 0.0f, -5.0f}},
        worldUp  {wu},
        yaw      {y},
        pitch    {p}
    {
        update();
    }

    glm::mat4 Camera::viewMatrix() const noexcept
    {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::processMouse(float x_offset, float y_offset) noexcept
    {
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw   += x_offset;
        pitch += y_offset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update();
    }

    void Camera::processKeyboard(Movement movement, float dt) noexcept
    {
        float velocity = speed * dt;

        if (movement == Forward)
            position += front * velocity;
        if (movement == Backward)
            position -= front * velocity;
        if (movement == Left)
            position -= right * velocity;
        if (movement == Right)
            position += right * velocity;
    }

    void Camera::update() noexcept
    {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(f);

        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }

}
