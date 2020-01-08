#pragma once

#include <memory>

#include <glm/glm.hpp>

namespace glem::render {

    class Drawable;
    class VertexArray;

    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
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
         * @brief release
         */
        static void release() noexcept;

    private:
        static std::shared_ptr<VertexArray> vao_;
        static Vertex*  buffer_;
        static uint32_t index_;

    };

}

