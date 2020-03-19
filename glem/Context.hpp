#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace glem {

    class Context {
    public:
        Context(GLFWwindow* parent);
        ~Context();

        Context(Context&&) = delete;
        Context(const Context&) = delete;

        Context& operator=(Context&&) = delete;
        Context& operator=(const Context&) = delete;

        void beginFrame(const glm::vec4& color) const noexcept;
        void endFrame() const noexcept;

        void drawIndexed(uint32_t size) noexcept;

    private:
        GLFWwindow* parent_ {nullptr};

    };

}
