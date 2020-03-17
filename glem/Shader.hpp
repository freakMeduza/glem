#pragma once

#include "Bindable.hpp"

#include <string>

#include <glad/glad.h>

namespace glem {

    enum class ShaderType {
        VS,
        PS
    };

    template<ShaderType Type> struct ShaderTypeMap;

    template<> struct ShaderTypeMap<ShaderType::VS> {
        static constexpr size_t type = GL_VERTEX_SHADER;
    };

    template<> struct ShaderTypeMap<ShaderType::PS> {
        static constexpr size_t type = GL_FRAGMENT_SHADER;
    };

    class Shader {
    public:
        Shader(const std::string& source, ShaderType type);
        ~Shader();

        Shader(Shader&&) = delete;
        Shader(const Shader&) = delete;

        Shader& operator=(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;

        /**
         * @brief Shader handler
         * @return
         */
        uint32_t handler() const noexcept;

    private:
        uint32_t handler_{0u};

        ShaderType type_;

    };

}
