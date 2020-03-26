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

            Log::e(TAG, "Unable to link program: ", msg);

            shaders_.clear();

            glDeleteProgram(handler_);

            return false;
        }

        shaders_.clear();

        return true;
    }

    bool Program::setUniform(const std::string &tag, int value) noexcept
    {
        if(auto location = resolveUniformLocation(tag)) {
            glUniform1i(*location, value);
            return true;
        }

        Log::e(TAG, "Failed to resolve uniform location: ", tag);

        return false;
    }

    bool Program::setUniform(const std::string &tag, float value) noexcept
    {
        if(auto location = resolveUniformLocation(tag)) {
            glUniform1f(*location, value);
            return true;
        }

        Log::e(TAG, "Failed to resolve uniform location: ", tag);

        return false;
    }

    bool Program::setUniform(const std::string &tag, const glm::vec3 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag)) {
            glUniform3f(*location, value.x, value.y, value.z);
            return true;
        }

        Log::e(TAG, "Failed to resolve uniform location: ", tag);

        return false;
    }

    bool Program::setUniform(const std::string &tag, const glm::vec4 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag)) {
            glUniform4f(*location, value.x, value.y, value.z, value.w);
            return true;
        }

        Log::e(TAG, "Failed to resolve uniform location: ", tag);

        return false;
    }

    bool Program::setUniform(const std::string &tag, const glm::mat4 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag)) {
            glUniformMatrix4fv(*location, 1, GL_FALSE, glm::value_ptr(value));
            return true;
        }

        Log::e(TAG, "Failed to resolve uniform location: ", tag);

        return false;
    }

    std::optional<GLint> Program::resolveUniformLocation(const std::string &value) noexcept {
        if(auto it = location.find(value); it != location.end())
            return location[value];

        if(auto l = glGetUniformLocation(handler_, value.c_str()); l != -1) {
            location[value] = l;
            return l;
        }

        return {};
    }

}
