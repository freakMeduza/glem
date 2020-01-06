#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include <optional>
#include <functional>

#include <render/context.hpp>

#include "event.hpp"

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

        /**
         * @brief nativeWindow
         * @return
         */
        GLFWwindow& nativeWindow() const noexcept;

        /**
         * @brief setEventCallBack
         * @param value
         */
        void setEventCallBack(const std::function<void(Event&)>& value) noexcept;

    private:
        GLFWwindow* window_ {nullptr};

        int width_  {0};
        int height_ {0};

        std::string title_;

        std::unique_ptr<render::Context> context_ {nullptr};

        std::function<void(Event&)> eventCallBack_;

    };

}
