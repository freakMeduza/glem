#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace glem::render {

    class Font;
    class Texture;
    class Drawable;
    class VertexArray;

    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;

        float unit;
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
         * @brief submitText
         * @param text
         * @param position
         * @param font
         * @param color
         * @param scale
         */
        static void submitText(const std::string& text, const glm::vec2& position, const Font& font, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float scale = 1.0f) noexcept;

        /**
         * @brief end
         */
        static void end() noexcept;

        /**
         * @brief present
         */
        static void present() noexcept;

    };

}

