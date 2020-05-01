#include "Application.hpp"
#include "Context.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Scene.hpp"
#include "Mesh.hpp"

#include "Log.hpp"
#include "Timer.hpp"

#include "Vertex.hpp"

namespace {
    static constexpr const char* TAG = "Application";
}

namespace glem {

    Application &Application::instance() noexcept {
        static Application app;

        return app;
    }

    Window &Application::window() const noexcept
    {
        return *window_;
    }

    Context &Application::context() const noexcept
    {
        return *context_;
    }

    int Application::exec() noexcept
    {
        Timer timer;

        std::unique_ptr<Scene> scene = std::make_unique<ParticleScene>();//= std::make_unique<DynamicVertexSystemTest>();

        auto model = Shape::cube();

        while(true) {
            if(auto ret = window_->pollEvents()) {
                /**** shitdown ****/

                return *ret;
            }

            auto deltaTime = timer.mark();

            if(Keyboard::pressed(Keyboard::Key::Escape))
                window_->close();

            if(scene) {
                scene->update(deltaTime);
                scene->render();
            }
            else {
                context_->beginFrame({0.1f, 0.1f, 0.1f, 1.0f});
                context_->endFrame();
            }
        }
    }

    bool Application::init() noexcept
    {
        if(!glfwInit()) {
            Log::e(TAG, "Failed to initialize GLFW.");
            return false;
        }

        window_ = std::make_unique<Window>();

        if(!window_) {
            Log::e(TAG, "Failed to create window.");
            return false;
        }

        context_ = std::make_unique<Context>(*window_);

        if(!context_) {
            Log::e(TAG, "Failed to create context.");
            return false;
        }

        return true;
    }

    Application::Application()
    {
        init();
    }

    Application::~Application()
    {

    }

}
