#include "camera.hpp"

namespace {
    const float NEAR = -1.0f;
    const float FAR  = 1.0f;
}

namespace glem::render {

    Camera::Camera(const glm::mat4 &projection) :
        view_       {glm::mat4(1.0f)},
        projection_ {projection}
    {
        viewProjection_ = projection_ * view_;
    }

    Camera::Camera(float left, float right, float bottom, float top) :
        view_       {glm::mat4(1.0f)},
        projection_ {glm::ortho(left, right, bottom, top, NEAR, FAR)}
    {
        viewProjection_ = projection_ * view_;
    }

    void Camera::setPosition(const glm::vec3 &value) noexcept
    {
        position_ = value;

        update();
    }

    const glm::vec3 &Camera::position() const noexcept
    {
        return position_;
    }

    void Camera::setSensitivity(float value) noexcept
    {
        sensitivity_ = value;
    }

    float Camera::sensitivity() const noexcept
    {
        return sensitivity_;
    }

    void Camera::move(Moveable::Movement movement, float dt) noexcept
    {
        switch (movement) {
        case Movement::Up:    { position_.y += sensitivity_ * dt; break; }
        case Movement::Down:  { position_.y -= sensitivity_ * dt; break; }
        case Movement::Left:  { position_.x -= sensitivity_ * dt; break; }
        case Movement::Right: { position_.x += sensitivity_ * dt; break; }
        }

        update();
    }

    void Camera::update() noexcept
    {
        // TODO: add rotation
        const auto& transform = glm::translate(glm::mat4(1.0f), position());

        view_           = glm::inverse(transform);
        viewProjection_ = projection_ * view_;
    }

    const glm::mat4 &Camera::view() const noexcept
    {
        return view_;
    }

    const glm::mat4 &Camera::projection() const noexcept
    {
        return projection_;
    }

    const glm::mat4 &Camera::viewProjection() const noexcept
    {
        return viewProjection_;
    }

}
