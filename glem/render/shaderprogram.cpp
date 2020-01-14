#include "shaderprogram.hpp"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <optional>

#include <util/log.hpp>

namespace {
    const std::string TAG = "ShaderProgram";
}

namespace glem::render {

    ShaderProgram::ShaderProgram()
    {
        id_ = glCreateProgram();
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(id_);
    }

    void ShaderProgram::bind(uint32_t unit) noexcept
    {
        static_cast<void>(unit);
        glUseProgram(id_);
    }

    void ShaderProgram::append(const std::shared_ptr<Shader> &value) noexcept
    {
        glAttachShader(id_, value->id());

        shaders_.emplace_back(std::move(value));
    }

    bool ShaderProgram::link() noexcept
    {
        glLinkProgram(id_);

        int ret {0};

        glGetProgramiv(id_, GL_LINK_STATUS, &ret);

        if(ret == GL_FALSE) {
            int length {0};

            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);

            std::string msg(std::size_t(length), ' ');

            glGetProgramInfoLog(id_, length, &length, msg.data());

            util::Log::e(TAG, "Unable to link program:", msg);

            shaders_.clear();

            glDeleteProgram(id_);

            return false;
        }

        shaders_.clear();

        return true;
    }

    bool ShaderProgram::setUniform(const std::string &tag, int value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniform1i(location, value);

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, int *value, int size) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniform1iv(location, size, value);

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, float value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniform1f(location, value);

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, const glm::vec2 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniform2f(location, value.x, value.y);

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, const glm::vec3 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniform3f(location, value.x, value.y, value.z);

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, const glm::vec4 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniform4f(location, value.x, value.y, value.z, value.w);

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, const glm::mat3 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    bool ShaderProgram::setUniform(const std::string &tag, const glm::mat4 &value) noexcept
    {
        if(auto location = resolveUniformLocation(tag); location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

            return true;
        }

        util::Log::e(TAG, "Failed to set uniform [", tag, "].");

        return false;
    }

    int ShaderProgram::resolveUniformLocation(const std::string &tag) noexcept
    {
        int result = -1;

        auto find = [this](const std::string& t)->std::optional<GLint>{
            auto location = glGetUniformLocation(id_, t.data());

            if(location == -1)
                return {};

            return location;
        };

        if(auto it = uniforms_.find(tag); it != uniforms_.end())
            result = (*it).second;
        else {
            if(auto l = find(tag)) {
                uniforms_[tag] = *l;
                result = *l;
            }
        }

        return result;
    }

}
