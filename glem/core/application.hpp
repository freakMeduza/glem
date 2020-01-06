#pragma once

#include <memory>

namespace glem::core {

    class Layer;
    class Window;

    class Application {
    public:
        virtual ~Application() = default;

        Application(Application&&) = delete;
        Application(const Application&) = delete;

        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;

        /**
         * @brief exec
         * @return
         */
        int exec() noexcept;

        /**
         * @brief window
         * @return
         */
        std::shared_ptr<Window> window() noexcept;

        /**
         * @brief instance
         * @return
         */
        static Application& instance() noexcept;

    private:
        void onStart() noexcept;
        void onShutdown() noexcept;

        Application() = default;

        std::shared_ptr<Window> window_ {nullptr};

    };

}
