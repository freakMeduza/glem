#pragma once

#include <memory>
#include <string>

namespace glem::render {

    /**
     * @brief The ShaderType enum
     *
     * PS Pixel (Fragment) sahder
     * VS Vertex shader
     * GS Geometry shader
     */
    enum ShaderType {
        PS,
        VS,
        GS
    };

    /**
     * @brief The Shader class
     *
     * Compile shader source (.glsl)
     */
    class Shader {
    public:
        Shader(const std::string& source, ShaderType type);
        ~Shader();

        Shader(Shader&&) = delete;
        Shader(const Shader&) = delete;

        Shader& operator=(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;

        /**
         * @brief fromFile      Compile shader from file
         * @param path          File path
         * @param type          Shader type
         * @return
         */
        static std::shared_ptr<Shader> fromFile(const std::string& path, ShaderType type) noexcept;

        /**
         * @brief fromSource    Compile shader from given source
         * @param source        Source
         * @param type          Shader type
         * @return
         */
        static std::shared_ptr<Shader> fromSource(const std::string& source, ShaderType type) noexcept;

        /**
         * @brief id            Shader object id
         * @return
         */
        [[nodiscard]] uint32_t id() const noexcept;

        /**
         * @brief type          Shader type
         * @return
         */
        [[nodiscard]] ShaderType type() const noexcept;

        /**
         * @brief source        Shader source
         * @return
         */
        [[nodiscard]] const std::string& source() const noexcept;

    private:
        uint32_t    id_ {0};
        ShaderType  type_;
        std::string source_;

    };

}
