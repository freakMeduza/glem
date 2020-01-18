#include <glem.hpp>

class Sandbox : public glem::core::Application {
public:
    ~Sandbox() override = default;

};

class DemoState : public glem::core::State {
public:
    DemoState() = default;
    ~DemoState() override = default;

    // State interface
    void onAttach() noexcept override {
        const auto& width  = glem::core::Application::instance().window()->width();
        const auto& height = glem::core::Application::instance().window()->height();

        camera_.reset(new glem::render::Camera{glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height))});

        glem::util::Log::d("DemoState", __FUNCTION__);
    }

    void onDetach() noexcept override {
        glem::util::Log::d("DemoState", __FUNCTION__);
    }

    void onEvent(glem::core::Event &event) noexcept override {
        glem::util::Log::d("DemoState", __FUNCTION__);
    }

    void onUpdate(float dt) noexcept override {
        static_cast<void>(dt);

        if(glem::core::InputManager::keyboard().isKeyPressed(glem::core::Keyboard::Escape))
            glem::core::Application::instance().window()->close();
    }

    void onDraw() noexcept override {
        glem::util::Log::d("DemoState", __FUNCTION__);
    }

private:
    std::unique_ptr<glem::render::Camera> camera_ {nullptr};

    std::vector<glem::render::Drawable> sprites_;

};

int main() {
    auto&& sandbox = Sandbox::instance();

    sandbox.push(std::make_shared<DemoState>());

    return sandbox.exec();
}
