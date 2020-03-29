#include "Shader.hpp"

#include "Log.hpp"

#include <glad/glad.h>

namespace {
    static constexpr const char* TAG = "Shader";
}

namespace glem {

    template<> struct ShaderTypeMap<ShaderType::VS> {
        static constexpr size_t type = GL_VERTEX_SHADER;
    };

    template<> struct ShaderTypeMap<ShaderType::PS> {
        static constexpr size_t type = GL_FRAGMENT_SHADER;
    };

    Shader::Shader(const std::string &source, ShaderType type) :
        type_{type}
    {
        handler_ = compile(source, type_);
    }

    Shader::~Shader()
    {
        glDeleteShader(handler_);
    }

    uint32_t Shader::handler() const noexcept
    {
        return handler_;
    }

    uint32_t Shader::compile(const std::string &src, ShaderType type) noexcept
    {
        uint32_t shader{0u};

        switch (type) {
        case ShaderType::VS:
            shader = glCreateShader(ShaderTypeMap<ShaderType::VS>::type);
            break;
        case ShaderType::PS:
            shader = glCreateShader(ShaderTypeMap<ShaderType::PS>::type);
            break;
        }

        auto csrc   = src.c_str();

        glShaderSource(shader, 1, &csrc, nullptr);

        glCompileShader(shader);

        int ret {GL_FALSE};

        glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);

        if(ret == GL_FALSE) {
            int length {0};

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::string msg(std::size_t(length), ' ');

            glGetShaderInfoLog(shader, length, &length, msg.data());

            glem::Log::e(TAG, "Unable to compile shader: ", msg, src);

            glDeleteShader(shader);

            return 0u;
        }

        return shader;
    }

}
