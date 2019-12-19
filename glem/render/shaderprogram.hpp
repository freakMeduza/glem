#pragma once

#include "shader.hpp"
#include "bindable.hpp"

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

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

        /**
         * @brief setUniform    Upload uniform into shader program
         * @param tag           Uniform value tag
         * @param value         Suppot value (int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4)
         * @return
         */
        bool setUniform(const std::string& tag, int value) noexcept;
        bool setUniform(const std::string& tag, float value) noexcept;
        bool setUniform(const std::string& tag, const glm::vec2& value) noexcept;
        bool setUniform(const std::string& tag, const glm::vec3& value) noexcept;
        bool setUniform(const std::string& tag, const glm::vec4& value) noexcept;
        bool setUniform(const std::string& tag, const glm::mat3& value) noexcept;
        bool setUniform(const std::string& tag, const glm::mat4& value) noexcept;

    private:
        int resolveUniformLocation(const std::string& tag) noexcept;

        std::vector<std::shared_ptr<Shader>> shaders_;

        std::unordered_map<std::string, int> uniforms_;

    };

}
