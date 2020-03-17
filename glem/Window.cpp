#include "Window.hpp"

namespace {
    const char* TITLE = "Glem Engine";

    const uint32_t WIDTH  = 1280u;
    const uint32_t HEIGHT = 720u;
}

namespace glem {

    Window::Window()
    {
        init();
    }

    Window::~Window()
    {
        glfwDestroyWindow(handler_);
    }

    uint32_t Window::width() const noexcept
    {
        return WIDTH;
    }

    uint32_t Window::height() const noexcept
    {
        return HEIGHT;
    }

    std::string Window::title() const noexcept
    {
        return TITLE;
    }

    std::optional<int> Window::pollEvents() const noexcept
    {
        glfwPollEvents();

        if(!glfwWindowShouldClose(handler_))
            return {};

        return 0;
    }

    void Window::close() const noexcept
    {
        glfwSetWindowShouldClose(handler_, GLFW_TRUE);
    }

    GLFWwindow *Window::handler() const noexcept
    {
        return handler_;
    }

    bool Window::init() noexcept
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

#ifdef NDEBUG
#else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        handler_ = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

        if(!handler_)
            return false;

        /**** move window to center ****/
        {
            const auto& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwSetWindowPos(handler_, (mode->width - WIDTH) / 2, (mode->height - HEIGHT) / 2);
        }

        return true;
    }

}
