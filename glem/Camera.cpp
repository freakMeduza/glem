#include "Camera.hpp"

#include "Log.hpp"
#include "Input.hpp"

#include <glad/glad.h>

#include "Application.hpp"
#include "Window.hpp"

namespace {
    const std::string TAG = "FreeCamera";

    [[maybe_unused]] static const glm::vec3 X_AXIS = {1.0f, 0.0f, 0.0f};
    [[maybe_unused]] static const glm::vec3 Y_AXIS = {0.0f, 1.0f, 0.0f};
    [[maybe_unused]] static const glm::vec3 Z_AXIS = {0.0f, 0.0f, 1.0f};

    [[maybe_unused]] static const glm::vec3 DEFAULT_CAMERA_POSITION = {0.0f, 0.0f, 40.0f};

    [[maybe_unused]] static const float DEFAULT_CAMERA_RADIUS      = 50.0f;
    [[maybe_unused]] static const float DEFAULT_CAMERA_PITCH       = 0.0f;
    [[maybe_unused]] static const float DEFAULT_CAMERA_YAW         = 0.0f;
    [[maybe_unused]] static const float DEFAULT_CAMERA_SENSITIVITY = 0.05f;
    [[maybe_unused]] static const float DEFAULT_CAMERA_SPEED       = 5.0f;
    [[maybe_unused]] static const float DEFAULT_CAMERA_SPRINT      = DEFAULT_CAMERA_SPEED * 2.0f;
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
    }

    FreeCamera::~FreeCamera()
    {
        Mouse::setCapture(false);
    }

    void FreeCamera::update(float deltaTime) noexcept
    {
        if(!Mouse::captured()) {
            Mouse::setCapture(true);

            double x {0.0};
            double y {0.0};

            glfwGetCursorPos(Application::instance().window().handler(), &x, &y);

            mouse_ = glm::vec2{static_cast<float>(x), static_cast<float>(y)};
        }

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

        auto speed = (Keyboard::isKeyPressed(Keyboard::Key::LeftShift) ? sprint_ : speed_) * deltaTime;

        if(Keyboard::isKeyPressed(Keyboard::Key::W))
            position_ += forward * speed;
        if(Keyboard::isKeyPressed(Keyboard::Key::S))
            position_ -= forward * speed;
        if(Keyboard::isKeyPressed(Keyboard::Key::A))
            position_ -= right * speed;
        if(Keyboard::isKeyPressed(Keyboard::Key::D))
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
        Mouse::setCapture(false);
    }

    void MayaCamera::update(float deltaTime) noexcept
    {
        if(Keyboard::isKeyPressed(Keyboard::Key::LeftAlt)) {
            Mouse::setCapture(true);

            auto mouse = Mouse::position();

            if(Mouse::isButtonPressed(Mouse::Button::ButtonLeft)) {
                auto offset = mouse - mouse_;

                mouse_ = mouse;

                yaw_   += offset.x * sensitivity_;
                pitch_ += offset.y * sensitivity_;
            }

            if(Mouse::isButtonPressed(Mouse::Button::ButtonRight)) {
                auto offset = mouse - mouse_;

                mouse_ = mouse;

                position_.z += offset.y * deltaTime;
            }

            mouse_ = mouse;
        }
        else
            Mouse::setCapture(false);

        auto orientation = glm::cross(glm::angleAxis(glm::radians(-pitch_), X_AXIS), glm::angleAxis(glm::radians(yaw_), Y_AXIS));

        view_ = glm::translate(-glm::vec3{0.0f, 0.0f, position_.z}) * glm::toMat4(orientation);
    }

}
