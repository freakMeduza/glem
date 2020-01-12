#pragma once

#include <memory>

#include <glm/glm.hpp>

namespace glem::render {

    class Texture;
    class Drawable;
    class VertexArray;

    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;

        float slot;
    };

    class Renderer {
    public:
        Renderer() = delete;
        ~Renderer() = delete;

        /**
         * @brief init
         */
        static void init() noexcept;

        /**
         * @brief deinit
         */
        static void deinit() noexcept;

        /**
         * @brief begin
         */
        static void begin() noexcept;

        /**
         * @brief submit
         * @param value
         */
        static void submit(const std::shared_ptr<Drawable>& value) noexcept;

        /**
         * @brief end
         */
        static void end() noexcept;

        /**
         * @brief present
         */
        static void present() noexcept;

        static void submitTextured(const glm::vec3 position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, int slot) noexcept;

    private:
        static VertexArray* vao_;
        static Vertex*      buffer_;
        static uint32_t     index_;

    };

}

