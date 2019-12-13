#pragma once

#include "window.hpp"

namespace glem::core {

    class Application {
    public:
        Application();
        ~Application();

        Application(Application&&) = delete;
        Application(const Application&) = delete;

        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;

        int exec() noexcept;

    private:
        std::unique_ptr<Window> window_ {nullptr};

    };

    Application* createApplication() noexcept;

}
