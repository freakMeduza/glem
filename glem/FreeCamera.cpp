#include "FreeCamera.hpp"

#include "Log.hpp"

#include "Mouse.hpp"
#include "Keyboard.hpp"

#include <glad/glad.h>

namespace {
    const std::string TAG = "FreeCamera";

    [[maybe_unused]] static glm::vec3 X_AXIS = {1.0f, 0.0f, 0.0f};
    [[maybe_unused]] static glm::vec3 Y_AXIS = {0.0f, 1.0f, 0.0f};
    [[maybe_unused]] static glm::vec3 Z_AXIS = {0.0f, 0.0f, 1.0f};

    [[maybe_unused]] static glm::vec3 DEFAULT_CAMERA_POSITION    = {0.0f, 0.0f, 10.0f};
    [[maybe_unused]] static glm::vec3 DEFAULT_CAMERA_ROTATION    = {90.0f, 0.0f, 0.0f};

    [[maybe_unused]] static float     DEFAULT_CAMERA_PITCH       = 0.0f;
    [[maybe_unused]] static float     DEFAULT_CAMERA_HEADING     = 90.0f;
    [[maybe_unused]] static float     DEFAULT_CAMERA_SENSITIVITY = 0.05f;
    [[maybe_unused]] static float     DEFAULT_CAMERA_SPEED       = 5.0f;
    [[maybe_unused]] static float     DEFAULT_CAMERA_SPRINT      = DEFAULT_CAMERA_SPEED * 2.0f;
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

    void FreeCamera::update() noexcept
    {
        auto forward = glm::normalize(point_ - position_);
        auto right   = glm::cross(forward, Y_AXIS);


        auto orientation = glm::normalize(glm::cross(glm::angleAxis(pitch_, X_AXIS), glm::angleAxis(heading_, Y_AXIS)));


    }

}
