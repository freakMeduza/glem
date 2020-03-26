#pragma once

#include <array>
#include <memory>

namespace glem {

    class Window;
    class Context;

    class Application {
    public:
        Application(Application&&) = delete;
        Application(const Application&) = delete;

        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;

        /**
         * @brief Application instance
         * @return
         */
        static Application& instance() noexcept;

        /**
         * @brief Application window
         * @return
         */
        Window& window() const noexcept;

        /**
         * @brief Application context
         * @return
         */
        Context& context() const noexcept;

        /**
         * @brief exec
         * @return
         */
        int exec() noexcept;

    private:
        bool init() noexcept;

        Application();
        ~Application();

        std::unique_ptr<Window>  window_  {nullptr};
        std::unique_ptr<Context> context_ {nullptr};

    };




}
