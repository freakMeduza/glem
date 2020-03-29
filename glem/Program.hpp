#pragma once

#include "Shader.hpp"
#include "Bindable.hpp"

#include <map>
#include <memory>
#include <vector>
#include <optional>

#include <glm/glm.hpp>

namespace glem {

    class Program : public Bindable {
    public:
        Program();
        ~Program() override;

        Program(Program&&) = delete;
        Program(const Program&) = delete;

        Program& operator=(Program&&) = delete;
        Program& operator=(const Program&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief Append shader to program
         * @param value - shader
         */
        void append(std::unique_ptr<Shader> value) noexcept;

        /**
         * @brief Link program
         * @return
         */
        bool link() const noexcept;

        /**
         * @brief Set uniform value
         * @param tag   - Uniform name
         * @param value - value (int, float, glm::vec3, glm::vec4, glm::mat4 supported)
         * @return
         */
        bool setUniform(const std::string& tag, int value) noexcept;
        bool setUniform(const std::string& tag, float value) noexcept;
        bool setUniform(const std::string& tag, const glm::vec3& value) noexcept;
        bool setUniform(const std::string& tag, const glm::vec4& value) noexcept;
        bool setUniform(const std::string& tag, const glm::mat4& value) noexcept;

    private:
        std::optional<int> resolveUniformLocation(const std::string& value) noexcept;

        std::map<std::string, int> location;

        mutable std::vector<std::unique_ptr<Shader>> shaders_;

    };

}
