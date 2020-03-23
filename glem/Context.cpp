#include "Context.hpp"
#include "Log.hpp"

#include <assert.h>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

namespace {
    const std::string TAG = "Context";

    void APIENTRY gl_debug_callback(GLenum        source,
                                    GLenum        type,
                                    GLuint        id,
                                    GLenum        severity,
                                    GLsizei       length,
                                    const GLchar* msg,
                                    const void*   userParam) {
        static_cast<void>(length);
        static_cast<void>(userParam);

        if(id == 131169 || id == 1311485 || id == 131218 || id == 131208)
            return;

        std::stringstream ss;

        ss << "[OpenGL DEBUG]: "
           << "("
           << id
           << ") ";

        switch (source) {
        case GL_DEBUG_SOURCE_API:             { ss << "[API] ";             break; }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   { ss << "[Window System] ";   break; }
        case GL_DEBUG_SOURCE_SHADER_COMPILER: { ss << "[Shader Compiler] "; break; }
        case GL_DEBUG_SOURCE_THIRD_PARTY:     { ss << "[Third Party] ";     break; }
        case GL_DEBUG_SOURCE_APPLICATION:     { ss << "[Application] ";     break; }
        case GL_DEBUG_SOURCE_OTHER:           { ss << "[Other] ";           break; }
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               { ss << "[Error] ";                break; }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: { ss << "[Deprecated Behaviour] "; break; }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  { ss << "[Undefined Behaviour] ";  break; }
        case GL_DEBUG_TYPE_PORTABILITY:         { ss << "[Portability] ";          break; }
        case GL_DEBUG_TYPE_PERFORMANCE:         { ss << "[Performance] ";          break; }
        case GL_DEBUG_TYPE_MARKER:              { ss << "[Marker] ";               break; }
        case GL_DEBUG_TYPE_PUSH_GROUP:          { ss << "[Push Group] ";           break; }
        case GL_DEBUG_TYPE_POP_GROUP:           { ss << "[Pop Group] ";            break; }
        case GL_DEBUG_TYPE_OTHER:               { ss << "[Other] ";                break; }
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         { ss << "[Hight] ";         break; }
        case GL_DEBUG_SEVERITY_MEDIUM:       { ss << "[Medium] ";        break; }
        case GL_DEBUG_SEVERITY_LOW:          { ss << "[Low] ";           break; }
        case GL_DEBUG_SEVERITY_NOTIFICATION: { ss << "[Notififcation] "; break; }
        }

        ss << msg;

        glem::Log::e(TAG, ss.str());
    }
}

namespace glem {

    Context::Context(GLFWwindow *parent) :
        parent_{parent}
    {
        assert(parent_ != nullptr);

        glfwMakeContextCurrent(parent_);

        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            Log::e(TAG, "Failed to initialize GLAD.");
            abort();
        }

        GLint flags;

        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

            glDebugMessageCallback(gl_debug_callback, nullptr);

            glDebugMessageControl(GL_DONT_CARE,
                                  GL_DEBUG_TYPE_ERROR,
                                  GL_DONT_CARE,
                                  0,
                                  nullptr,
                                  GL_TRUE);
        }

        auto vendor   = glGetString(GL_VENDOR);
        auto renderer = glGetString(GL_RENDERER);
        auto version  = glGetString(GL_VERSION);

        Log::d(TAG, reinterpret_cast<const char*>(vendor));
        Log::d(TAG, reinterpret_cast<const char*>(renderer));
        Log::d(TAG, "OpenGL: ", reinterpret_cast<const char*>(version));

        /**** viewport ****/
        {
            int w {0};
            int h {0};

            glfwGetWindowSize(parent_, &w, &h);

            glViewport(0, 0, w, h);
        }

        glfwSwapInterval(1);
    }

    Context::~Context()
    {

    }

    void Context::beginFrame(const glm::vec4 &color) const noexcept
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Context::endFrame() const noexcept
    {
        glfwSwapBuffers(parent_);
    }

    void Context::drawIndexed(size_t size) noexcept
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(size), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    }

}
