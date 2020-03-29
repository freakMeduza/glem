#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace glem {

    class Window;

    class Context {
    public:
        Context(Window& parent);
        ~Context();

        Context(Context&&) = delete;
        Context(const Context&) = delete;

        Context& operator=(Context&&) = delete;
        Context& operator=(const Context&) = delete;

        /**
         * @brief Begin frame
         * @param color
         */
        void beginFrame(const glm::vec4& color) const noexcept;

        /**
         * @brief End frame
         */
        void endFrame() const noexcept;

        /**
         * @brief Render indexed
         * @param size     - number of indicies
         * @param topology - topology
         */
        void renderIndexed(size_t size, GLenum topology = GL_TRIANGLES) noexcept;

    private:
        Window& parent_;

    };

}
