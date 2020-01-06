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

        glfwSetWindowUserPointer(window_, &eventCallBack_);

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            static_cast<void>(window);
            static_cast<void>(scancode);
            static_cast<void>(mods);

            if(auto function = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window))) {
                KeyboardEvent e{(action == GLFW_PRESS || action == GLFW_REPEAT ? Event::KeyPressed : Event::KeyReleased),
                                static_cast<uint16_t>(key)};

                function(e);
            }
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods){
            static_cast<void>(mods);

            double x;
            double y;

            glfwGetCursorPos(window, &x, &y);

            if(auto function = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window))) {
                MouseEvent e{(action == GLFW_PRESS ? Event::MouseButtonPressed : Event::MouseButtonReleased),
                             static_cast<uint16_t>(button),
                             static_cast<float>(x),
                             static_cast<float>(y)};
                function(e);
            }
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos){
            if(auto function = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window))) {
                MouseEvent e{Event::MouseMoved,
                             static_cast<uint16_t>(0),
                             static_cast<float>(xpos),
                             static_cast<float>(ypos)};
                function(e);
            }
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset){
            if(auto function = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window))) {
                double x {0.0};
                double y {0.0};

                glfwGetCursorPos(window, &x, &y);

                MouseEvent e{Event::MouseScrolled,
                             static_cast<uint16_t>(0),
                             static_cast<float>(x),
                             static_cast<float>(y),
                             static_cast<float>(xoffset),
                             static_cast<float>(yoffset)};
                function(e);
            }
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

    GLFWwindow &Window::nativeWindow() const noexcept
    {
        return *window_;
    }

    void Window::setEventCallBack(const std::function<void (Event &)> &value) noexcept
    {
        eventCallBack_ = value;
    }

}
