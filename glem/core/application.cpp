#include "window.hpp"
#include "application.hpp"
#include "inputmanager.hpp"
#include "statemanager.hpp"

#include <util/log.hpp>
#include <util/timer.hpp>

#include <glad/glad.h>

#include "demostate.hpp"

namespace {
    const std::string TAG = "Application";
}

namespace glem::core {

    int Application::exec() noexcept
    {
        onStart();

        util::Timer markTimer;

        float dt = 0.0f;

        while(true) {
            dt = markTimer.mark();

            if(auto ret = window_->pollEvents()) {
                // Cleanup
                onShutdown();

                return *ret;
            }

            if(!StateManager::empty())
                StateManager::top()->onUpdate(dt);

            window_->context().beginFrame();

            if(!StateManager::empty())
                StateManager::top()->onDraw();

            window_->context().endFrame();
        }
    }

    std::shared_ptr<Window> Application::window() noexcept
    {
        return window_;
    }

    Application &Application::instance() noexcept
    {
        static Application app;

        return app;
    }

    void Application::onStart() noexcept
    {
        window_.reset(new Window{});

        window_->setEventCallBack([](Event& event){ if(!StateManager::empty()) StateManager::top()->onEvent(event); });

        StateManager::push(std::make_shared<DemoState>());
    }

    void Application::onShutdown() noexcept
    {
        while(!StateManager::empty())
            StateManager::pop();
    }
}
