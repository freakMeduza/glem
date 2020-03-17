#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glem {

    class Context {
    public:
        Context(GLFWwindow* parent);
        ~Context();

        Context(Context&&) = delete;
        Context(const Context&) = delete;

        Context& operator=(Context&&) = delete;
        Context& operator=(const Context&) = delete;

    private:
        GLFWwindow* parent_ {nullptr};

    };

}
