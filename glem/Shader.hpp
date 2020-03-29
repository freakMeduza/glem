#pragma once

#include "Bindable.hpp"

#include <string>

namespace glem {

    enum class ShaderType {
        VS,
        PS
    };

    template<ShaderType> struct ShaderTypeMap;

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
        uint32_t compile(const std::string& src, ShaderType type) noexcept;

        uint32_t handler_{0u};

        ShaderType type_;

    };

}
