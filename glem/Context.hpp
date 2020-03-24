#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glad/glad.h>

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

        void renderIndexed(size_t size, GLint topology = GL_TRIANGLES) noexcept;

    private:
        GLFWwindow* parent_ {nullptr};

    };

}
