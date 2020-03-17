#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glem {

    class Input {
    public:
        Input() = delete;
        ~Input() = delete;

        Input(Input&&) = delete;
        Input(const Input&) = delete;

        Input& operator=(Input&&) = delete;
        Input& operator=(const Input&) = delete;

        static void setParent(GLFWwindow* parent) noexcept {
            parent_ = parent;
        }

    protected:
        static GLFWwindow* parent_;

    };



}
