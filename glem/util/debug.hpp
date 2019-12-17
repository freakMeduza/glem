#pragma once

#include <glad/glad.h>

#include <sstream>
#include <iostream>

#define UNUSED(x) static_cast<void>((x));

void APIENTRY glDebugOutput(GLenum        source,
                            GLenum        type,
                            GLuint        id,
                            GLenum        severity,
                            GLsizei       length,
                            const GLchar* msg,
                            const void*   userParam) {
    if(id == 131169 || id == 1311485 || id == 131218 || id == 131208)
        return;

    UNUSED(length)
    UNUSED(userParam)

    std::stringstream ss;

    ss << "[OpenGL DEBUG]: "
       << "("
       << id
       << ") ";

    switch (source) {
    case GL_DEBUG_SOURCE_API:             { ss << "[Source]: API ";             break; }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   { ss << "[Source]: Window System ";   break; }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: { ss << "[Source]: Shader Compiler "; break; }
    case GL_DEBUG_SOURCE_THIRD_PARTY:     { ss << "[Source]: Third Party ";     break; }
    case GL_DEBUG_SOURCE_APPLICATION:     { ss << "[Source]: Application ";     break; }
    case GL_DEBUG_SOURCE_OTHER:           { ss << "[Source]: Other ";           break; }
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               { ss << "[Type]: Error ";                break; }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: { ss << "[Type]: Deprecated Behaviour "; break; }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  { ss << "[Type]: Undefined Behaviour ";  break; }
    case GL_DEBUG_TYPE_PORTABILITY:         { ss << "[Type]: Portability ";          break; }
    case GL_DEBUG_TYPE_PERFORMANCE:         { ss << "[Type]: Performance ";          break; }
    case GL_DEBUG_TYPE_MARKER:              { ss << "[Type]: Marker ";               break; }
    case GL_DEBUG_TYPE_PUSH_GROUP:          { ss << "[Type]: Push Group ";           break; }
    case GL_DEBUG_TYPE_POP_GROUP:           { ss << "[Type]: Pop Group ";            break; }
    case GL_DEBUG_TYPE_OTHER:               { ss << "[Type]: Other ";                break; }
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         { ss << "[Severity]: Hight ";         break; }
    case GL_DEBUG_SEVERITY_MEDIUM:       { ss << "[Severity]: Medium ";        break; }
    case GL_DEBUG_SEVERITY_LOW:          { ss << "[Severity]: Low ";           break; }
    case GL_DEBUG_SEVERITY_NOTIFICATION: { ss << "[Severity]: Notififcation "; break; }
    }

    ss << "[" << msg << "]";

    std::cerr << ss.str() << std::endl;
}

