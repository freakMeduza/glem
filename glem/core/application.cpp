#include "application.hpp"

#include <iostream>

namespace glem::core {

    Application::Application() :
        window_ {new Window{}}
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    Application::~Application()
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    int Application::exec() noexcept
    {
        while(true) {
            if(auto ret = window_->pollEvents())
                return *ret;

            window_->context().beginFrame();



            window_->context().endFrame();
        }
    }

}
