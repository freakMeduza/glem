#include "keyboard.hpp"

#include "window.hpp"
#include "application.hpp"

#include <GLFW/glfw3.h>

namespace glem::core {

    bool Keyboard::isKeyPressed(Keyboard::Key key) const noexcept
    {
        auto state = glfwGetKey(&Application::instance().window()->nativeWindow(), static_cast<uint16_t>(key));

        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

}
