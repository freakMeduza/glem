#include "window.hpp"
#include "inputmanager.hpp"

#include <glad/glad.h>

#include <util/log.hpp>

namespace {
    const std::string TAG = "Window";

    const int   DEFAULT_WIDTH  = 1280;
    const int   DEFAULT_HEIGHT = 720;
    const char* DEFAULT_TITLE  = "GLEM Engine";
}

namespace glem::core {

    Window::Window() :
        width_  {DEFAULT_WIDTH},
        height_ {DEFAULT_HEIGHT},
        title_  {DEFAULT_TITLE}
    {
        if(!glfwInit()) {
            util::Log::e(TAG, "Failed to initialize GLFW.");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

#ifdef NDEBUG
#else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        window_ = glfwCreateWindow(width_, height_, title_.data(), nullptr, nullptr);

        if(!window_) {
            util::Log::e(TAG, "Failed to create GLFW window.");
            return;
        }

        const auto& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwSetWindowPos(window_, (mode->width - width_) / 2, (mode->height - height_) / 2);

        context_.reset(new render::Context{window_});

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            static_cast<void>(window);
            static_cast<void>(scancode);
            static_cast<void>(mods);

            InputManager::onKeyboardEvent(KeyboardEvent{(action == GLFW_PRESS ? Action::Pressed : Action::Released),
                                                        static_cast<uint16_t>(key)});
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods){
            static_cast<void>(mods);

            double x;
            double y;

            glfwGetCursorPos(window, &x, &y);

            InputManager::onMouseEvent(MouseEvent{(action == GLFW_PRESS ? Action::Pressed : Action::Released),
                                                  static_cast<int>(x),
                                                  static_cast<int>(y),
                                                  0.0f,
                                                  static_cast<uint16_t>(button)});
        });
    }

    Window::~Window()
    {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    int Window::width() const noexcept
    {
        return width_;
    }

    int Window::height() const noexcept
    {
        return height_;
    }

    std::string Window::title() const noexcept
    {
        return title_;
    }

    std::optional<int> Window::pollEvents() noexcept
    {
        glfwPollEvents();

        if(!glfwWindowShouldClose(window_))
            return {};

        return 0;
    }

    void Window::close() const noexcept
    {
        glfwSetWindowShouldClose(window_, true);
    }

    render::Context &Window::context() const noexcept
    {
        return *context_;
    }

}
