#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <optional>

namespace glem {

    class Window {
    public:
        Window();
        ~Window();

        Window(Window&&) = delete;
        Window(const Window&) = delete;

        Window& operator=(Window&&) = delete;
        Window& operator=(const Window&) = delete;

        /**
         * @brief Window width
         * @return
         */
        uint32_t width() const noexcept;

        /**
         * @brief Window height
         * @return
         */
        uint32_t height() const noexcept;

        /**
         * @brief Window title
         * @return
         */
        std::string title() const noexcept;

        /**
         * @brief Poll events from window
         * @return
         */
        std::optional<int> pollEvents() const noexcept;

        /**
         * @brief Close window
         */
        void close() const noexcept;

        /**
         * @brief Set cursor visibility mode
         * @param enable
         */
        void setCursorMode(bool enable) const noexcept;

        /**
         * @brief Window handler
         * @return
         */
        GLFWwindow* handler() const noexcept;

    private:
        bool init() noexcept;

        GLFWwindow* handler_ {nullptr};

    };

}
