#include <glem.hpp>

#include "demostate.hpp"

class Sandbox : public glem::core::Application {
public:
    ~Sandbox() override = default;

};

int main() {
    auto&& sandbox = Sandbox::instance();

    sandbox.push(std::make_shared<DemoState>());

    return sandbox.exec();
}
