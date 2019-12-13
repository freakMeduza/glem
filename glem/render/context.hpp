#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glem::render {

    class Context {
    public:
        Context(GLFWwindow* parent);
        ~Context();

        Context(Context&&) = delete;
        Context(const Context&) = delete;

        Context& operator=(Context&&) = delete;
        Context& operator=(const Context&) = delete;

        void beginFrame() noexcept;
        void endFrame() noexcept;

    private:
        GLFWwindow* parent_ {nullptr};

        bool vsync_ {true};

    };

}
