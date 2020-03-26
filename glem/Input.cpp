#include "Input.hpp"
#include "Window.hpp"
#include "Application.hpp"

namespace glem {

    bool Mouse::pressed(Mouse::Button value) noexcept
    {
        auto state = glfwGetMouseButton(Application::instance().window().handler(), static_cast<int>(value));

        return (state == GLFW_PRESS);
    }

    glm::vec2 Mouse::position() noexcept
    {
        double x{0.0};
        double y{0.0};

        glfwGetCursorPos(Application::instance().window().handler(), &x, &y);

        return { static_cast<float>(x), static_cast<float>(y) };
    }

    bool Keyboard::pressed(Keyboard::Key value) noexcept
    {
        auto state = glfwGetKey(Application::instance().window().handler(), static_cast<int>(value));

        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

}
