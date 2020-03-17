#include "Mouse.hpp"

#include <GLFW/glfw3.h>

namespace glem {

    bool Mouse::isButtonPressed(Mouse::Button value) noexcept
    {
        auto state = glfwGetMouseButton(parent_, static_cast<int>(value));

        return (state == GLFW_PRESS);
    }

    glm::vec2 Mouse::position() noexcept
    {
        double x{0.0};
        double y{0.0};

        glfwGetCursorPos(parent_, &x, &y);

        return { static_cast<float>(x), static_cast<float>(y) };
    }

}
