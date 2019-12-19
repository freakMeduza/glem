#pragma once

#include "shader.hpp"
#include "bindable.hpp"

#include <vector>

namespace glem::render {

    class ShaderProgram : public Bindable {
    public:
        ShaderProgram();
        ~ShaderProgram() override;

        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram(const ShaderProgram&) = delete;

        ShaderProgram& operator=(ShaderProgram&&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        // Bindable interface
        void bind() noexcept override;

        /**
         * @brief append        Append shader into program
         * @param value
         */
        void append(const std::shared_ptr<Shader>& value) noexcept;

        /**
         * @brief link          Link program
         * @return
         */
        bool link() noexcept;

    private:
        std::vector<std::shared_ptr<Shader>> shaders_;

    };

}
