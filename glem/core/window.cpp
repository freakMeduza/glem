#include "window.hpp"

#include <glad/glad.h>

#include <iostream>

namespace {
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
            std::cerr << "Failed to initialize GLFW." << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        window_ = glfwCreateWindow(width_, height_, title_.data(), nullptr, nullptr);

        if(!window_) {
            std::cerr << "Failed to create GLFW window." << std::endl;
            return;
        }

        const auto& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwSetWindowPos(window_, (mode->width - width_) / 2, (mode->height - height_) / 2);

        context_.reset(new render::Context{window_});

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            /**** TODO: event system ****/
            static_cast<void>(scancode);
            static_cast<void>(mods);

            if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
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

    render::Context &Window::context() const noexcept
    {
        return *context_;
    }

}
