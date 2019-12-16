#include "context.hpp"

#include <glad/glad.h>

#include <assert.h>
#include <iostream>

namespace {
    [[maybe_unused]] void info() noexcept {
        auto vendor   = glGetString(GL_VENDOR);
        auto renderer = glGetString(GL_RENDERER);
        auto version  = glGetString(GL_VERSION);

        std::cout << "OpenGL Context:" << std::endl;
        std::cout << "[vendor]:   " << vendor   << std::endl;
        std::cout << "[version]:  " << version  << std::endl;
        std::cout << "[renderer]: " << renderer << std::endl;
    }
}

namespace glem::render {

    Context::Context(GLFWwindow *parent) : parent_ {parent}
    {
        assert(parent_ != nullptr);

        glfwMakeContextCurrent(parent);

        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD." << std::endl;
            return;
        }

        info();

        int width  {0};
        int height {0};

        glfwGetWindowSize(parent_, &width, &height);

        glViewport(0, 0, width, height);

        /**** vsync enabled ****/
        glfwSwapInterval(1);
    }

    Context::~Context()
    {

    }

    void Context::beginFrame() noexcept
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    void Context::endFrame() noexcept
    {
        glfwSwapBuffers(parent_);
    }

}
