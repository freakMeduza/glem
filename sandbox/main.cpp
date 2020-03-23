#include <Application.hpp>

int main() {
    auto& app = glem::Application::instance();

    return app.exec();
}

