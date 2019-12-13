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

        int width()  const noexcept;
        int height() const noexcept;

        std::string title() const noexcept;

        std::optional<int> pollEvents() noexcept;

        render::Context& context() const noexcept;

    private:
        GLFWwindow* window_ {nullptr};

        int width_  {0};
        int height_ {0};

        std::string title_;

        std::unique_ptr<render::Context> context_ {nullptr};

    };

}
