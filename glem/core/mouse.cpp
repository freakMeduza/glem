#include "mouse.hpp"

#include "window.hpp"
#include "application.hpp"

#include "GLFW/glfw3.h"

namespace glem::core {

    bool Mouse::isButtonPressed(Mouse::Button button) const noexcept
    {
        auto state = glfwGetMouseButton(&Application::instance().window()->nativeWindow(), static_cast<uint16_t>(button));

        return (state == GLFW_PRESS);
    }

    std::pair<float, float> Mouse::position() const noexcept
    {
        double x {0.0};
        double y {0.0};

        glfwGetCursorPos(&Application::instance().window()->nativeWindow(), &x, &y);

        return {static_cast<float>(x), static_cast<float>(y)};
    }

}
