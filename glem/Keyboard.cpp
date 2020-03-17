#include "Keyboard.hpp"

namespace glem {

    bool Keyboard::isKeyPressed(Keyboard::Key value) noexcept
    {
        auto state = glfwGetKey(parent_, static_cast<int>(value));

        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

}
