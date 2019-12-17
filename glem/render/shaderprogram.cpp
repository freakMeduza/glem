#include "shaderprogram.hpp"

#include <glad/glad.h>

#include <iostream>

namespace glem::render {

    ShaderProgram::ShaderProgram()
    {
        id_ = glCreateProgram();
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(id_);
    }

    void ShaderProgram::bind() noexcept
    {
        glUseProgram(id_);
    }

    void ShaderProgram::onAppend(Visitor &visitor) noexcept
    {
        visitor.visit(*this);
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

            std::cerr << "Unable to link program: " << msg << std::endl;

            shaders_.clear();

            glDeleteProgram(id_);

            return false;
        }

        shaders_.clear();

        return true;
    }

}
