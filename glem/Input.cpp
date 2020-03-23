#include "Input.hpp"

#include "Application.hpp"
#include "Window.hpp"

namespace {
    static bool captured_ {false};
}

namespace glem {

    GLFWwindow* Input::parent_ = nullptr;

    void Mouse::setCapture(bool value) noexcept
    {
        if(!captured_)
            Application::instance().window().setCursorMode(!value);
        else
            Application::instance().window().setCursorMode(!value);

        captured_ = value;
    }

    bool Mouse::captured() noexcept
    {
        return captured_;
    }

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

    bool Keyboard::isKeyPressed(Keyboard::Key value) noexcept
    {
        auto state = glfwGetKey(parent_, static_cast<int>(value));

        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

}
