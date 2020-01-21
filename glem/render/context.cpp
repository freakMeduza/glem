#include "context.hpp"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>

#include <util/debug.hpp>

namespace {
    [[maybe_unused]] void info() noexcept {
        auto vendor   = glGetString(GL_VENDOR);
        auto renderer = glGetString(GL_RENDERER);
        auto version  = glGetString(GL_VERSION);

        std::cout << vendor    << std::endl;
        std::cout << "OpenGL " << version << std::endl;
        std::cout << renderer  << std::endl;
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

        /**** Enable debug context ****/
        GLint flags;

        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);

            glDebugMessageControl(GL_DONT_CARE,
                                  GL_DEBUG_TYPE_ERROR,
                                  GL_DONT_CARE,
                                  0,
                                  nullptr,
                                  GL_TRUE);
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

    void Context::enableBlend() noexcept
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Context::disableBlend() noexcept
    {
        glDisable(GL_BLEND);
    }

}
