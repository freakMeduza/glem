#include <glem.hpp>

class Sandbox : public glem::core::Application {
public:
    ~Sandbox() override = default;

};

int main() {
    auto&& sandbox = Sandbox::instance();

    return sandbox.exec();
}
