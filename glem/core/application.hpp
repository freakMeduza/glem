#pragma once

#include <memory>

namespace glem::core {

    class State;
    class Window;

    class Application {
    public:
        virtual ~Application() = default;

        Application(Application&&) = delete;
        Application(const Application&) = delete;

        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;

        /**
         * @brief push
         * @param value
         */
        void push(const std::shared_ptr<State>& value) noexcept;

        /**
         * @brief pop
         */
        void pop() noexcept;

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
        Application();

        std::shared_ptr<Window> window_ {nullptr};

    };

}
