#include <glem.hpp>

class Sandbox : public glem::core::Application {
public:
    ~Sandbox() override = default;

};

int main() {
    return Sandbox::instance().exec();
}
