#include "Camera.hpp"

#include "Log.hpp"
#include "Input.hpp"

#include "Application.hpp"
#include "Window.hpp"

namespace {
    [[maybe_unused]] static constexpr const glm::vec3 X_AXIS = {1.0f, 0.0f, 0.0f};
    [[maybe_unused]] static constexpr const glm::vec3 Y_AXIS = {0.0f, 1.0f, 0.0f};
    [[maybe_unused]] static constexpr const glm::vec3 Z_AXIS = {0.0f, 0.0f, 1.0f};

    [[maybe_unused]] static constexpr const float DEFAULT_CAMERA_RADIUS      = 50.0f;
    [[maybe_unused]] static constexpr const float DEFAULT_CAMERA_PITCH       = 0.0f;
    [[maybe_unused]] static constexpr const float DEFAULT_CAMERA_YAW         = 0.0f;
    [[maybe_unused]] static constexpr const float DEFAULT_CAMERA_SENSITIVITY = 0.05f;
    [[maybe_unused]] static constexpr const float DEFAULT_CAMERA_SPEED       = 10.0f;
    [[maybe_unused]] static constexpr const float DEFAULT_CAMERA_SPRINT      = DEFAULT_CAMERA_SPEED * 2.0f;
}

namespace glem {

    /**** Free camera ****/
    FreeCamera::FreeCamera() :
        sensitivity_ {DEFAULT_CAMERA_SENSITIVITY},
        speed_       {DEFAULT_CAMERA_SPEED},
        sprint_      {DEFAULT_CAMERA_SPRINT},
        pitch_       {DEFAULT_CAMERA_PITCH},
        yaw_         {DEFAULT_CAMERA_YAW}
    {
        glfwSetCursorPos(Application::instance().window().handler(),
                         Application::instance().window().width()  / 2.0f,
                         Application::instance().window().height() / 2.0f);

        double x {0.0};
        double y {0.0};

        glfwGetCursorPos(Application::instance().window().handler(), &x, &y);

        mouse_ = glm::vec2{static_cast<float>(x), static_cast<float>(y)};

        glfwSetInputMode(Application::instance().window().handler(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    FreeCamera::~FreeCamera()
    {

    }

    void FreeCamera::update(float deltaTime) noexcept
    {
        auto mouse  = Mouse::position();
        auto offset = glm::vec2{mouse.x - mouse_.x, mouse_.y - mouse.y};

        mouse_ = mouse;

        yaw_   += offset.x * sensitivity_;
        pitch_ += offset.y * sensitivity_;

        if(pitch_ >=  89.0f)
            pitch_ =  89.0f;
        if(pitch_ <= -89.0f)
            pitch_ = -89.0f;

        auto orientation = glm::cross(glm::angleAxis(glm::radians(-pitch_), X_AXIS), glm::angleAxis(glm::radians(yaw_), Y_AXIS));

        auto v = view();

        auto forward = -glm::vec3{v[0][2], v[1][2], v[2][2]};
        auto right   =  glm::vec3{v[0][0], v[1][0], v[2][0]};

        auto speed = (Keyboard::pressed(Keyboard::Key::LeftShift) ? sprint_ : speed_) * deltaTime;

        if(Keyboard::pressed(Keyboard::Key::W))
            position_ += forward * speed;
        if(Keyboard::pressed(Keyboard::Key::S))
            position_ -= forward * speed;
        if(Keyboard::pressed(Keyboard::Key::A))
            position_ -= right * speed;
        if(Keyboard::pressed(Keyboard::Key::D))
            position_ += right * speed;

        view_ = glm::toMat4(orientation) * glm::translate(-position_);
    }

    /**** Maya camera ****/
    MayaCamera::MayaCamera() :
        sensitivity_ {DEFAULT_CAMERA_SENSITIVITY},
        pitch_       {DEFAULT_CAMERA_PITCH},
        yaw_         {DEFAULT_CAMERA_YAW}
    {
        double x {0.0};
        double y {0.0};

        glfwGetCursorPos(Application::instance().window().handler(), &x, &y);

        mouse_ = glm::vec2{static_cast<float>(x), static_cast<float>(y)};

        position_.z = DEFAULT_CAMERA_RADIUS;
    }

    MayaCamera::~MayaCamera()
    {

    }

    void MayaCamera::update(float deltaTime) noexcept
    {
        if(Keyboard::pressed(Keyboard::Key::LeftAlt)) {
            glfwSetInputMode(Application::instance().window().handler(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            auto mouse = Mouse::position();

            if(Mouse::pressed(Mouse::Button::ButtonLeft)) {
                auto offset = mouse - mouse_;

                mouse_ = mouse;

                yaw_   += offset.x * sensitivity_;
                pitch_ += offset.y * sensitivity_;
            }

            if(Mouse::pressed(Mouse::Button::ButtonRight)) {
                auto offset = mouse - mouse_;

                mouse_ = mouse;

                position_.z += offset.y * deltaTime;
            }

            mouse_ = mouse;
        }
        else
            glfwSetInputMode(Application::instance().window().handler(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        auto orientation = glm::cross(glm::angleAxis(glm::radians(-pitch_), X_AXIS), glm::angleAxis(glm::radians(yaw_), Y_AXIS));

        view_ = glm::translate(-glm::vec3{0.0f, 0.0f, position_.z}) * glm::toMat4(orientation);
    }

}
