#include "FreeCamera.hpp"

#include "Log.hpp"
#include "Input.hpp"

#include <glad/glad.h>

namespace {
    const std::string TAG = "FreeCamera";

    [[maybe_unused]] static glm::vec3 X_AXIS = {1.0f, 0.0f, 0.0f};
    [[maybe_unused]] static glm::vec3 Y_AXIS = {0.0f, 1.0f, 0.0f};
    [[maybe_unused]] static glm::vec3 Z_AXIS = {0.0f, 0.0f, 1.0f};

    [[maybe_unused]] static glm::vec3 DEFAULT_CAMERA_POSITION = {0.0f, 0.0f, 40.0f};

    [[maybe_unused]] static float DEFAULT_CAMERA_PITCH       = 0.0f;
    [[maybe_unused]] static float DEFAULT_CAMERA_HEADING     = 90.0f;
    [[maybe_unused]] static float DEFAULT_CAMERA_SENSITIVITY = 0.05f;
    [[maybe_unused]] static float DEFAULT_CAMERA_SPEED       = 5.0f;
    [[maybe_unused]] static float DEFAULT_CAMERA_SPRINT      = DEFAULT_CAMERA_SPEED * 2.0f;

    [[maybe_unused]] static std::string to_string(const glm::vec3& value) noexcept {
        std::stringstream ss;
        ss << value.x << " " << value.y << " " << value.z;

        return ss.str();
    }
}

namespace glem {

    FreeCamera::FreeCamera() :
        sensitivity_ {DEFAULT_CAMERA_SENSITIVITY},
        speed_       {DEFAULT_CAMERA_SPEED},
        sprint_      {DEFAULT_CAMERA_SPRINT},
        pitch_       {DEFAULT_CAMERA_PITCH},
        heading_     {DEFAULT_CAMERA_HEADING}
    {

    }

    FreeCamera::~FreeCamera()
    {

    }

    void FreeCamera::focus() noexcept
    {
        /**** disable cursor ****/
    }

    void FreeCamera::update(float deltaTime) noexcept
    {
        auto forward = glm::normalize(lookAt_ - position_);
        auto right   = glm::cross(forward, Y_AXIS);

        Log::d(TAG, to_string(forward));
        Log::d(TAG, to_string(right));

        auto speed = (Keyboard::isKeyPressed(Keyboard::Key::LeftShift) ? sprint_ : speed_) * deltaTime;

        if(Keyboard::isKeyPressed(Keyboard::Key::W))
            position_ += forward * speed;
        else if(Keyboard::isKeyPressed(Keyboard::Key::S))
            position_ -= forward * speed;

//        if(Keyboard::isKeyPressed(Keyboard::Key::A))
//            position_ -= right * speed;
//        else if(Keyboard::isKeyPressed(Keyboard::Key::D))
//            position_ += right * speed;


    }

}
