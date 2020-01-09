#include "window.hpp"
#include "application.hpp"
#include "inputmanager.hpp"
#include "statemanager.hpp"
#include "state.hpp"

#include <render/renderer.hpp>

#include <util/log.hpp>
#include <util/timer.hpp>

#include <glad/glad.h>

namespace {
    const std::string TAG = "Application";
}

namespace glem::core {

    void Application::push(const std::shared_ptr<State> &value) noexcept
    {
        StateManager::push(value);
    }

    void Application::pop() noexcept
    {
        StateManager::pop();
    }

    int Application::exec() noexcept
    {
        render::Renderer::init();

        util::Timer markTimer;

        float dt = 0.0f;

        while(true) {
            if(StateManager::empty()) {
                util::Log::w(TAG, "State stack is empty.");

                return 0;
            }

            dt = markTimer.mark();

            if(auto ret = window_->pollEvents()) {
                while(!StateManager::empty())
                    StateManager::pop();

                render::Renderer::deinit();

                return *ret;
            }

            StateManager::top()->onUpdate(dt);

            window_->context().beginFrame();

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

    Application::Application() :
        window_ {new Window{}}
    {
        window_->setEventCallBack([](Event& event){ StateManager::top()->onEvent(event); });
    }
}
