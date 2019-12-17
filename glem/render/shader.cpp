#include "shader.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace {
    [[maybe_unused]] GLenum translate(glem::render::ShaderType t) noexcept {
        switch (t) {
        case glem::render::ShaderType::PS: { return GL_FRAGMENT_SHADER; }
        case glem::render::ShaderType::VS: { return GL_VERTEX_SHADER;   }
        case glem::render::ShaderType::GS: { return GL_GEOMETRY_SHADER; }
        }

        return GL_FALSE;
    }

    [[maybe_unused]] auto compile(const std::string& src, glem::render::ShaderType type) noexcept {
        auto shader = glCreateShader(translate(type));
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

            std::cerr << "Unable to compile shader: " << msg << std::endl;
            std::cerr << src << std::endl;

            glDeleteShader(shader);

            return 0u;
        }

        return shader;
    }

    [[maybe_unused]] const std::string readFile(const std::string& path) noexcept {
        std::ifstream file{path, std::ios::in | std::ios::binary};

        if(file.is_open()) {
            std::stringstream ss;

            ss << file.rdbuf();

            file.close();

            return ss.str();
        }

        std::cerr << "Unable to open file for reading ." << path << std::endl;

        return {};
    }

}

namespace glem::render {

    Shader::Shader(const std::string &source, ShaderType type) :
        type_   {type},
        source_ {source}
    {
        id_ = compile(source_, type);
    }

    Shader::~Shader()
    {
        glDeleteShader(id_);
    }

    std::shared_ptr<Shader> Shader::fromFile(const std::string &path, ShaderType type) noexcept
    {
        auto&& src = readFile(path);

        return fromSource(std::move(src), type);
    }

    std::shared_ptr<Shader> Shader::fromSource(const std::string &source, ShaderType type) noexcept
    {
        return std::make_shared<Shader>(std::move(source), type);
    }

    uint32_t Shader::id() const noexcept
    {
        return id_;
    }

    ShaderType Shader::type() const noexcept
    {
        return type_;
    }

    const std::string &Shader::source() const noexcept
    {
        return source_;
    }

}
