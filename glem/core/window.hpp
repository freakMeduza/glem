#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include <optional>

#include <render/context.hpp>

namespace glem::core {

    class Window {
    public:
        Window();
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        /**
         * @brief width
         * @return
         */
        int width()  const noexcept;

        /**
         * @brief height
         * @return
         */
        int height() const noexcept;

        /**
         * @brief title
         * @return
         */
        std::string title() const noexcept;

        /**
         * @brief pollEvents
         * @return
         */
        std::optional<int> pollEvents() noexcept;

        /**
         * @brief close
         */
        void close() const noexcept;

        /**
         * @brief context
         * @return
         */
        render::Context& context() const noexcept;

    private:
        GLFWwindow* window_ {nullptr};

        int width_  {0};
        int height_ {0};

        std::string title_;

        std::unique_ptr<render::Context> context_ {nullptr};

    };

}
