#include "Shader.hpp"

#include "Log.hpp"

namespace {
    const std::string TAG = "Shader";

    [[maybe_unused]] auto compile(const std::string& src, glem::ShaderType type) noexcept {
        uint32_t shader{0u};

        switch (type) {
        case glem::ShaderType::VS:
            shader = glCreateShader(glem::ShaderTypeMap<glem::ShaderType::VS>::type);
            break;
        case glem::ShaderType::PS:
            shader = glCreateShader(glem::ShaderTypeMap<glem::ShaderType::PS>::type);
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

            glem::Log::e(TAG, "Unable to compile shader:", msg, src);

            glDeleteShader(shader);

            return 0u;
        }

        return shader;
    }
}

namespace glem {

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

}
