#include "Program.hpp"

#include "Log.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace {
    const std::string TAG = "Program";
}

namespace glem {

    Program::Program()
    {
        handler_ = glCreateProgram();
    }

    Program::~Program()
    {
        glDeleteProgram(handler_);
    }

    void Program::bind() const noexcept
    {
        glUseProgram(handler_);
    }

    void Program::unbind() const noexcept
    {
        glUseProgram(0);
    }

    void Program::append(std::unique_ptr<Shader> value) noexcept
    {
        glAttachShader(handler_, value->handler());

        shaders_.emplace_back(std::move(value));
    }

    bool Program::link() const noexcept
    {
        glLinkProgram(handler_);

        int ret {0};

        glGetProgramiv(handler_, GL_LINK_STATUS, &ret);

        if(ret == GL_FALSE) {
            int length {0};

            glGetProgramiv(handler_, GL_INFO_LOG_LENGTH, &length);

            std::string msg(std::size_t(length), ' ');

            glGetProgramInfoLog(handler_, length, &length, msg.data());

            Log::e(TAG, "Unable to link program:", msg);

            shaders_.clear();

            glDeleteProgram(handler_);

            return false;
        }

        shaders_.clear();

        return true;
    }

    bool Program::setUniform(const std::string &tag, const glm::vec3 &value) noexcept
    {
        auto location = glGetUniformLocation(handler_, tag.data());

        if(location == -1) {
            Log::e(TAG, "Fuck off!");
            return false;
        }

        glUniform3f(location, value.x, value.y, value.z);

        return true;
    }

    bool Program::setUniform(const std::string &tag, const glm::mat4 &value) noexcept
    {
        auto location = glGetUniformLocation(handler_, tag.data());

        if(location == -1) {
            Log::e(TAG, "Fuck off!");
            return false;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

        return true;
    }

}
